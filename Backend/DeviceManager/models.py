from django.db import models

# Create your models here.


class Room(models.Model):
    number = models.IntegerField()
    occupied = models.BooleanField()
    lastActivity = models.DateField(auto_now=True)
    

    def __str__(self):
        return f"Room no.{self.number}"

class Device(models.Model):
    CHOICES=[
        ('OCCUPANT','OCCUPANT'),
        ('ADMIN','ADMIN')
    ]
    deviceIDRef = models.CharField(max_length=20,default="01XYZ")
    ledState = models.BooleanField()
    roomNumber=models.ForeignKey("Room",on_delete=models.CASCADE)
    lastToggle = models.DateField( auto_now=True)
    lastUser = models.CharField(max_length=10,choices=CHOICES)
    

    def __str__(self):
        return self.deviceIDRef
   
