from django.urls.conf import path
from .views import DeviceView, DeviceUpdateView, DeviceDevView

urlpatterns = [
    path("",DeviceView.as_view(),name="device"),
    path("<int:device_id>/",
         DeviceDevView.as_view(), name="device_post"),
    path("update/<int:device_id>/", 
        DeviceUpdateView.as_view(),name="device_update"),
]
