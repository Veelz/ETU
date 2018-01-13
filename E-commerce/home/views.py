from django.utils import timezone
from django.http import Http404
from django.shortcuts import render, get_object_or_404
from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger
from products.models import ProductImage, ProductCategory, Product


def homepage(request):
    product_images = ProductImage.objects.filter(product__is_active=True, is_active=True, is_main=True)
    new_date = timezone.now() -  timezone.timedelta(days=30)
    categories_to_show = ProductCategory.objects.all()[:3]

    product_images_new = product_images.filter(
        product__create_date__gt=new_date).order_by('-create_date')[:3]
    product_images_cat_1 = product_images.filter(product__category=categories_to_show[0],
        product__create_date__gt=new_date).order_by('-create_date')[:3]
    product_images_cat_2 = product_images.filter(product__category=categories_to_show[1],
        product__create_date__gt=new_date).order_by('-create_date')[:3]

    args = {
        'product_images_new': product_images_new,
        'product_images_cat_1_name': categories_to_show[0].name,
        'product_images_cat_1': product_images_cat_1,
        'product_images_cat_2_name': categories_to_show[1].name,
        'product_images_cat_2': product_images_cat_2,
    }

    return render(request, 'home/home.html', args)

def category(request, cat_id, page_number=1):
    cat = get_object_or_404(ProductCategory, id=cat_id)
    product_images = ProductImage.objects.filter(product__is_active=True,
        product__category=cat, is_active=True, is_main=True).order_by('id')
    pagination = Paginator(product_images, 9)
    try:
        page = pagination.page(page_number)
    except PageNotAnInteger:
        page = pagination.page(1)
    except EmptyPage:
        page = pagination.page(pagination.num_pages)

    args = {
        'product_images' : page,
        'category_name' : cat.name,
        'category_id' : cat.id,
    }
    return render(request, 'home/category.html', args)

def search(request):
    products_sets = None
    paginate_by = 10
    query = request.GET.get('search')
    if query:
        query_list = query.split()
        products_sets = [Product.objects.filter(name__icontains=q, is_active=True) for q in query_list]
        if not products_sets[0]:
            products_sets = None
    args = {
        'products_sets' : products_sets,
        'query_text': query,
    }
    return render(request, 'home/search.html', args)
