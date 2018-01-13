

def get_cart(request):
    session_key = request.session.session_key
    cart = request.session.get(session_key, [])
    products_count = len(cart)
    total_price = '%.2f' % (sum([float(item['product_price']) * item['count'] for item in cart]))
    return locals()
