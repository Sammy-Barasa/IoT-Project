from django.urls.conf import path
from .views import DeviceView, DeviceUpdateView

urlpatterns = [
    path("",DeviceView.as_view()),
    path("update/", DeviceUpdateView.as_view()),
]
