from django.shortcuts import render, redirect
from django.http import HttpResponseForbidden
from django.contrib import auth
from django.contrib.auth.forms import UserCreationForm
from orders.models import Order, ProductInOrder
from .forms import UserCreationForm

def register(request):
    next_url = '/'
    user_form = UserCreationForm()
    if request.POST:
        data = request.POST
        new_user_form = UserCreationForm(data)
        if new_user_form.is_valid():
            new_user_form.save()
            username = new_user_form.cleaned_data['username']
            passwd = new_user_form.cleaned_data['password2']
            new_user = auth.authenticate(username=username, password=passwd)
            auth.login(request, new_user)
            return redirect(next_url)
        else:
            user_form = new_user_form
    return render(request, 'accounts/register.html', {'user_form': user_form})

def login(request):
    next_url = '/'
    if request.POST:
        data = request.POST
        session_key = request.session.session_key
        # print(session_key)
        if not session_key:
            request.session.cycle_key()
            session_key = request.session.session_key
        cart = request.session.get(session_key, [])
        username = data.get('username')
        password = data.get('password')
        user = auth.authenticate(username=username, password=password)
        if user is not None:
            auth.login(request, user)
            session_key = request.session.session_key
            request.session[session_key] = cart

            return redirect(next_url)
        else:
            login_error = 'User not found'
            return render(request, 'accounts/login.html', {'login_error' : login_error})

    return render(request, 'accounts/login.html')

def logout(request):
    next_url = '/'
    auth.logout(request)
    return redirect(next_url)

def account(request):
    if request.user.is_authenticated:
        args = {}
        username = request.user.username
        orders = Order.objects.filter(user__username=username).order_by('-create_date')
        args['orders'] = orders
        for order in args['orders']:
            products = ProductInOrder.objects.filter(order=order)
            order.products = products
        return render(request, 'accounts/account.html', args)

    return HttpResponseForbidden()

# def order_details(request):
#     return render()
