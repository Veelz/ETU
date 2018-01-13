from django.contrib import admin
from django.contrib.auth.admin import UserAdmin as BaseUserAdmin
from django.contrib.auth.models import User

from .models import Comment
from products.models import Product


class CommentAdmin(admin.ModelAdmin):
    list_display = [field.name for field in Comment._meta.fields]

    class Meta:
        model = Comment


admin.site.register(Comment, CommentAdmin)


class CommentInline(admin.StackedInline):
    model = Comment
    can_delete = False


class UserAdmin(BaseUserAdmin):
    inlines = [CommentInline]


admin.site.unregister(User)
admin.site.register(User, UserAdmin)
