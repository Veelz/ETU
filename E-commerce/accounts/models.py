from django.db import models
from django.contrib.auth.models import User
from products.models import Product

class Comment(models.Model):
    user = models.ForeignKey(User, blank=True, null=True, default=None)
    product = models.ForeignKey(Product, blank=True, null=True, default=None)
    comment_text = models.TextField(max_length=300, blank=True, default='')
    create_date = models.DateTimeField(auto_now_add=True, auto_now=False)
    update_date = models.DateTimeField(auto_now_add=False, auto_now=True)

    def __str__(self):
        return '%d. %s' % (self.product.id, self.product.name)

    class Meta:
        verbose_name = 'Комментарий к товару'
        verbose_name_plural = 'Комментарии к товарам'
