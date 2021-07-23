from django.urls.conf import path
from .views import index_view

urlpatterns = [
    path("",index_view),
]
