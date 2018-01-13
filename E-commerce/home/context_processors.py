from products.models import ProductCategory, Product


def get_category_list(request):
    categories = ProductCategory.objects.all()
    for item in categories:
        item.count = len(Product.objects.filter(category=item))
    return locals()
