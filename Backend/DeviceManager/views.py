from django.shortcuts import render
from DeviceManager.serializers import DeviceSerializer, DevicePostSerializer,DeviceUpdateSerializer
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
        return queryset

    def get(self, request):
        serializer = self.serializer_class(self.get_queryset(), many=True)
        return Response(data=serializer.data, status=status.HTTP_200_OK)
    

    
class DeviceDevView(generics.GenericAPIView):
    serializer_class = DevicePostSerializer

    # overriding get queryset
    def get_queryset(self):
        """
        returns specific user device
        """
        queryset = Device.objects.all()
        device_id = self.args['device_id']
        return queryset.filter(id=device_id)

    def post(self, request, device_id, devREF, *args, **kwargs):
        data = request.data
        dev = self.get_queryset()
        serializer = self.serializer_class(
            data=data, context={'request': request})
        serializer.is_valid(raise_exception=True)
        try:
            serializer.save()
            print(serializer.data)
            return Response(data={"message": "device has been added"}, status=status.HTTP_200_OK)
        except ValidationError as error:
            print(error)
            print(serializer.errors)
            return Response(data={"message": serializer.errors}, status=status.HTTP_400_BAD_REQUEST)

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
        queryset = Device.objects.get(id=id)
        return queryset

    # get specific
    def get(self, request, device_id):
        serializer = self.serializer_class(self.get_queryset())
        return Response(data=serializer.data, status=status.HTTP_200_OK)

    # update person
    def put(self, request, device_id):
        
        data = request.data
        serializer = self.serializer_class(self.get_queryset(), data=data)
        serializer.is_valid(raise_exception=True)
        serializer.save()
        print(serializer.data)
        return Response(data={"message": "device has been updated"}, status=status.HTTP_200_OK)
