from django.shortcuts import render
from DeviceManager.serializers import DeviceSerializer, DeviceUpdateSerializer
from DeviceManager.models import Device
from rest_framework.response import Response
from rest_framework import generics, status

# Create your views here.



# create ,get device API viw
class DeviceView(generics.GenericAPIView):
    serializer_class = DeviceSerializer

    # overriding get queryset
    def get_queryset(self):
        """
        returns specific user device
        """
        queryset = Device.objects.all()
        id = self.kwargs['device_id']
        ref = self.kwargs['devREF']
        device = Device.objects.get(pk=id)
        if device is not None:
            return queryset.filter(deviceIDRef=ref)

    def get(self, request, device_id,devREF):
        serializer = self.serializer_class(self.get_queryset(), many=True)
        return Response(data=serializer.data, status=status.HTTP_200_OK)
    def post(self, request,device_id,devREF):
        data = request.data
        serializer = self.serializer_class(
            data=data, context={'request': request})
        serializer.is_valid(raise_exception=True)
        try:
            serializer.save()
            print(serializer.data)
            return Response(data={"message": "device has been added"}, status=status.HTTP_201_CREATED)
        except ValidationError as error:
            print(error)
            print(serializer.errors)
            return Response(data={"message": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)

    # update device
    def put(self, request, device_id, devREF):
        data = request.data
        serializer = self.serializer_class(self.get_queryset(), data=data)
        serializer.is_valid(raise_exception=True)
        serializer.save()
        print(serializer.data)
        return Response(data={"message": "device has been updated"}, status=status.HTTP_200_OK)


class DeviceUpdateView(generics.RetrieveUpdateDestroyAPIView):
    
    serializer_class = DeviceUpdateSerializer
    lookup_field = "id"

    # overriding get queryset

    def get_queryset(self):
        """
        returns specific device for updated(put) 
        """
        id = self.kwargs['device_id']
        print(id)
        ref = self.kwargs['devREF']
        queryset = Device.objects.get(id=id)
        return queryset

    # update person
    def put(self, request, device_id, devREF):
        
        data = request.data
        serializer = self.serializer_class(self.get_queryset(), data=data)
        serializer.is_valid(raise_exception=True)
        serializer.save()
        print(serializer.data)
        return Response(data={"message": "device has been updated"}, status=status.HTTP_200_OK)
