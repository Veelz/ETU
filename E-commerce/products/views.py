from django.shortcuts import render, get_object_or_404
from django.http import HttpResponseForbidden, Http404, JsonResponse
from django.utils import dateformat

from products.models import *
from accounts.models import Comment

def product(request, product_id):
    args = {}
    args['product'] = get_object_or_404(Product, id=product_id, is_active=True)
    args['comments'] = Comment.objects.filter(product__id=product_id).order_by('create_date')
    # print(args['comments'])
    return render(request, 'products/product.html', args)

def add_comment(request, product_id):
    if request.POST:
        if not request.user.is_authenticated():
            return HttpResponseForbidden()
        user = request.user
        data = request.POST
        product = get_object_or_404(Product, id=product_id, is_active=True)
        text = data.get('text')
        comment = Comment.objects.create(user=user, product=product, comment_text=text)
        username = comment.user.username
        if hasattr(user, 'first_name'):
            username = comment.user.first_name
            if hasattr(user, 'last_name'):
                username += ' ' + comment.user.last_name
        args = {
            'username': username,
            'date': dateformat.format(comment.create_date, 'd E Y г.'),
            'comment_text': comment.comment_text,
        }
        return JsonResponse(args)

    return Http404('No data provided')
