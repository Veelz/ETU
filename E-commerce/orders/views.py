from decimal import *
from django.shortcuts import render, redirect
from django.http import JsonResponse, Http404
from django.urls import reverse
from products.models import Product
from orders.models import Order, ProductInOrder, Status


def update_cart(request):
    data = request.POST.copy()
    try:
        product_id = int(data.get('product_id'))
        product = Product.objects.get(id=product_id, is_active=True)
    except ValueError:
        raise Http404("Product does not exist")
    except TypeError:
        raise Http404("Product does not exist")
    except Product.DoesNotExist:
        raise Http404("Product does not exist")

    try:
        delete_item = (data.get('delete_item') == 'true')
    except TypeError:
        raise Http404("Attribute error")
    except ValueError:
        raise Http404("Attribute error")

    session_key = request.session.session_key
    if not session_key:
        request.session.cycle_key()
        session_key = request.session.session_key

    if delete_item:
        cart_products = delete_from_cart(request, session_key, product)
    else:
        try:
            count = int(data.get('count'))
            if count <= 0:
                raise ValueError
        except TypeError:
            count = 1
        except ValueError:
            count = 1
        cart_products = add_to_cart(request, session_key, product, count)

    request.session[session_key] = cart_products
    args = {
        "products" : cart_products,
        "product_count" : len(cart_products),
        "total_price": ('%.2f' % sum([float(item['total_price']) for item in cart_products])),
    }
    return JsonResponse(args)

def add_to_cart(request, session_key, product, count):
    product_dict = {}
    product_dict['product_id'] = product.id
    product_dict['product_name'] = product.name
    product_dict['count'] = count
    product_dict['product_price'] = "%.2f" % (product.price)

    cart_products = request.session.get(session_key, [])
    if cart_products:
        in_cart = False
        for item in cart_products:
            if item['product_id'] == product.id:
                in_cart = True
                item['count'] += count
        if not in_cart:
            cart_products += [product_dict]
    else:
        cart_products = [product_dict]

    for item in cart_products:
        total = float(item['product_price']) * float(item['count'])
        item['total_price'] = '%.2f' % (total)

    return cart_products

def delete_from_cart(request, session_key, product):
    cart_products = request.session.get(session_key)
    return [item for item in cart_products if item['product_id'] != product.id]

def checkout(request):
    if not request.POST:
        return render(request, 'orders/checkout.html')
    if not request.user.is_authenticated():
        return redirect(reverse('login'))

    data = request.POST.copy()
    session_key = request.session.session_key
    cart_products = request.session.get(session_key)
    user = request.user

    status_new = Status.objects.get(name='new', is_active=True)
    order = Order.objects.create(user=user, status=status_new)

    # key == name, value == count
    for key, value in data.items():
        if key.startswith('product_'):
            product_id = int(key.split('product_')[1])
            product_count = int(value)
            product_to_add = Product.objects.get(id=product_id, is_active=True)
            ProductInOrder.objects.create(order=order, product=product_to_add,
                count=product_count, price_per_item=product_to_add.price)

    request.session[session_key] = []
    return redirect(reverse('account'))
