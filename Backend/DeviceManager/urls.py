from django.urls.conf import path
from .views import DeviceView, DeviceUpdateView

urlpatterns = [
    path("<int:device_id>/<str:devRef>/",DeviceView.as_view(),name="device"),
    path("update/<int:device_id>/<str:devRef>/", DeviceUpdateView.as_view(),name="device_update"),
]
