from django.db import models

# Create your models here.

class Device(models.Model):
    CHOICES=[
        ('OCCUPANT','OCCUPANT'),
        ('ADMIN','ADMIN')
    ]
    ledState = models.BooleanField()
    roomNumber=models.IntegerField()
    lastToggle = models.DateField( auto_now=True)
    lastUser = models.CharField(max_length=10,choices=CHOICES)
    

    def __str__(self):
        return self.roomNumber
   