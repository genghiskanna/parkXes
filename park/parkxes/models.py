# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.contrib.auth.models import User
from django.db.models import Model,OneToOneField,EmailField,CharField,BooleanField, IntegerField, ForeignKey, CASCADE
from django.contrib.postgres.fields import ArrayField

class ParkingZone(Model):
    zone = CharField(max_length=50)
    comment = CharField(max_length=255)
    owner = ForeignKey(User,on_delete=CASCADE,default=None)
    editor = ArrayField(ArrayField(IntegerField()),default=[])


class CarUser(Model):
    vehicleType = CharField(max_length=20)
    name = CharField(max_length=30)
    designation = CharField(max_length=30)
    mobileNumber = CharField(max_length=10,primary_key=True)

class ParkingLocation(Model):
    lotId = CharField(max_length=4, primary_key=True)
    zone = ForeignKey(ParkingZone, on_delete=CASCADE)
    isParked = BooleanField(default=False)

class PreferredParkingLocation(Model):
    preferredParkingLocation = ForeignKey(ParkingLocation, on_delete=CASCADE)
    user = ForeignKey(CarUser, on_delete=CASCADE)

class Vehicle(Model):
    licensePlateNumber = CharField(max_length=10, primary_key=True)
    vehicleType = CharField(max_length=20)
    caruser = ForeignKey(CarUser, on_delete=CASCADE)


class Camera(Model):
    cameraName = CharField(max_length=100)
    cameraId = CharField(max_length=100, primary_key=True)
    comment = CharField(max_length=100)
    zone = ForeignKey(ParkingZone, on_delete=CASCADE)
    owner = ForeignKey(User,on_delete=CASCADE,default=None)



    
    


    
    
 