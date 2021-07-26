from rest_framework import serializers
from DeviceManager.models import Device

class DeviceSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = ['id', 'deviceIDRef', 'ledState',
                  'roomNumber', 'lastToggle', 'lastUser']
        read_only_fields = ['id']
        
        # validate

        def validate(self, attr):
            return attr

        def update(self, instance, validated_data):
            instance.ledState = validated_data.get('ledState')
            instance.lastToggle = validated_data.get('lastToggle')
            instance.lastUser = validated_data.get('lastUser')
            return instance

class DeviceUpdateSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = ['id', 'deviceIDRef', 'ledState',
                  'roomNumber', 'lastToggle', 'lastUser']
        read_only_fields = ['id', 'deviceIDRef']


        # validate

        def validate(self, attr):
            return attr

        # update
        def update(self, instance, validated_data):
            instance.ledState = validated_data.get('ledState')
            instance.lastToggle = validated_data.get('lastToggle')
            instance.lastUser = validated_data.get('lastUser')
            return instance
