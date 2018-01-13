from django.contrib import auth


def get_username(request):
    args = {}
    user = auth.get_user(request)
    if user is not None:
        has_name = False
        if hasattr(user, 'first_name'):
            args['username'] = user.first_name
            has_name = True
        if hasattr(user, 'last_name'):
            args['username'] += ' ' + user.last_name
            has_name = True
        if not has_name:
            args['username'] = user.username
    return args
