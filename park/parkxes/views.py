# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.contrib.auth.models import User
from parkxes.models import ParkingZone,Camera,ParkingLocation, CarUser, Vehicle, PreferredParkingLocation
from django.shortcuts import render,redirect
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate, login, logout
from DatabaseFunctions import *

def index(request):
    return render(request, 'index/index.html')

def loginIndex(request):
    return render(request, 'login/login.html')

def logoutIndex(request):
    logout(request)
    
    return redirect("/")

@csrf_exempt
def register(request):
    return render(request, 'login/register.html')

@csrf_exempt
def add_user_new(request):
    email = request.POST.get('email', None)
    password = request.POST.get('password', None)
    print email,password
    print "hello"
    user, created = User.objects.get_or_create(username=email, email=email)
    if created:
        user.set_password(password)
    user.save()
    return render(request, 'login/login.html')


@csrf_exempt
def check_user(request):
    email = request.POST.get('email', None)
    password = request.POST.get('password', None)
    user = authenticate(request, username=email, password=password)
    if user is not None:
        login(request, user)
        return redirect("/")
    else:
        print("Wrong password username combo")
        return render(request, 'login/login.html',{'failure': True})


def dashboard(request):
    if not request.user.is_authenticated():
            return redirect("/")
    else:
        parkSpace = []
        camera1 = []
        lotParked = []
        lotFree = 0
        try:
            for loop in Camera.objects.all():
                if loop.owner.username == request.user.username:
                    camera1.append(loop)
            for loop in ParkingZone.objects.all():
                if loop.owner.username == request.user.username:
                    parkSpace.append(loop)
            for loop in ParkingLocation.objects.all():
                if loop.isParked == False:
                    lotFree += 1
                else:
                    lotParked.append(loop.lotId)

        except ParkingZone.DoesNotExist:
            parkSpace = []
            print "Error in Dashboard"
        if len(Camera.objects.all()) != 0:
            return render(request, 'dashboard/dashboard.html',{'parkSpace': parkSpace,'camera':camera1[0],'lot':lotFree,'parked':lotParked})
        else:
            return render(request, 'dashboard/dashboard.html',{'parkSpace': parkSpace, 'lot': lotFree, 'parked': lotParked})


def editor(request):
    parkingZoneId = request.path[6:]
    try:
        lots = []
        for loop in ParkingZone.objects.all():
            if loop.id == int(parkingZoneId):
                lots = loop.editor
    except ParkingZone.DoesNotExist:
        lots = []

    for loop in ParkingLocation.objects.all():
        if loop.zone.id == int(parkingZoneId):
            if loop.isParked is True:
                row = int(loop.lotId[0])
                col = int(loop.lotId[1])
                lots[row][col] = 3

    return render(request, 'dashboard/editor.html',{'lots': lots,'zoneid':int(parkingZoneId)})


def userArea(request):
    try:
        lots = {}
        temp = []
        senp = []
        for subyloop in PreferredParkingLocation.objects.all():
            senp.append(subyloop.preferredParkingLocation.lotId)
        for loop in ParkingZone.objects.all():
            if loop.owner == request.user:
                for subloop in ParkingLocation.objects.all():
                    if loop.id == subloop.zone.id and subloop.isParked == False:
                        if len(senp) != 0:
                            if subloop.lotId not in senp:
                                temp.append(subloop.lotId)
                        else:
                            temp.append(subloop.lotId)
                lots[loop.zone] = temp
                temp = []
    except ParkingZone.DoesNotExist or ParkingLocation.DoesNotExist:
        lots = []
    return  render(request,'login/user_info.html',{'lots':lots})


def book(request):
    try:
        lot = request.POST.get('freespace', None)
        zone = request.POST.get('parkZone', None)
        mobileno = request.POST.get('mobile', None)
        print lot,zone,mobileno
        for loop in CarUser.objects.all():
            if (loop.mobileNumber == str(mobileno)) and (loop.designation == "teacher"):
                for subloop in ParkingLocation.objects.all():
                    if subloop.lotId == lot:
                        if len(PreferredParkingLocation.objects.all()) != 0:
                            for s in PreferredParkingLocation.objects.all():
                                if s.user != loop:
                                    newPrefParkLoc = PreferredParkingLocation()
                                    newPrefParkLoc.preferredParkingLocation = subloop
                                    newPrefParkLoc.user = loop
                                    newPrefParkLoc.save()
                                    return HttpResponse("Booked")
                        else:
                            newPrefParkLoc = PreferredParkingLocation()
                            newPrefParkLoc.preferredParkingLocation = subloop
                            newPrefParkLoc.user = loop
                            newPrefParkLoc.save()
                            return HttpResponse("Booked")

            elif loop.mobileNumber == mobileno and loop.designation == "student":
                print "hel"
                for sloop in ParkingLocation.objects.all():
                    if len(PreferredParkingLocation.objects.all()) != 0:
                        for subloop in PreferredParkingLocation.objects.all():
                            if sloop.lotId != subloop.preferredParkingLocation.lotId and sloop.isParked is False:
                                print "hel"
                                newPrefParkLoc = PreferredParkingLocation()
                                newPrefParkLoc.preferredParkingLocation = sloop
                                newPrefParkLoc.user = loop
                                newPrefParkLoc.save()
                                return render(request, 'login/allocate.html', {'loty': sloop.lotId})
                    else:
                        if sloop.isParked is False:
                            print "hel"
                            newPrefParkLoc = PreferredParkingLocation()
                            newPrefParkLoc.preferredParkingLocation = sloop
                            newPrefParkLoc.user = loop
                            newPrefParkLoc.save()
                            return render(request, 'login/allocate.html', {'loty': sloop.lotId})
            else:
                pass

    except ParkingZone.DoesNotExist or ParkingLocation.DoesNotExist:
        lots = []
    return  redirect("/user/")


def new_user(request):
    return render(request,'login/newuser.html')


# add Operations
def add_caruser(request):
    license_number = request.POST.get('license', None)
    name = request.POST.get('nameu', None)
    designation = request.POST.get('designation', None)
    mobile = request.POST.get('mobile', None)

    newUser = CarUser()
    newUser.mobileNumber = mobile
    newUser.designation = designation
    newUser.name = name
    newUser.save()
    license = license_number.split(",")
    for loop in license:
        for subloop in CarUser.objects.all():
            if subloop.mobileNumber == mobile:
                newCar = Vehicle()
                newCar.licensePlateNumber = loop
                newCar.vehicleType = "car"
                newCar.caruser = subloop
                newCar.save()

    return redirect("/user/")


def add_camera(request):
    
    cameraName = request.POST.get('camera_name', None)
    comment = request.POST.get('comment', None)
    cameraSelection = request.POST.get('cameraSelection', None)
    parkZone = int(request.POST.get('parkZone', None))

    for loop in ParkingZone.objects.all():
        if loop.id == parkZone:
            newCam = Camera()
            newCam.cameraName = cameraName
            newCam.comment = comment
            newCam.cameraId = cameraSelection
            newCam.zone = loop
            newCam.owner = request.user

            newCam.save()
    return redirect("/dashboard/")


def add_parking_location(request):
    if not request.user.is_authenticated():
            return redirect("/")
    else:
        zoneName = request.POST.get('zone_name',None)
        comment = request.POST.get('comment_zone',None)

        newZone = ParkingZone()
        newZone.zone = zoneName
        newZone.comment = comment
        newZone.owner = request.user

        newZone.save()
        return redirect("/dashboard/")

        
    
def add_lots(request):
    if not request.user.is_authenticated():
            return redirect("/")
    else:
        zoneId = request.POST.get('zoneid',None)
        editArray = request.POST.getlist('editArray[][]',None)
        editor_data = []
        oldZone = ParkingZone.objects.get(id=zoneId)
        # delete old parking lots
        ParkingLocation.objects.filter(zone=oldZone).delete()
        for loop,i in enumerate(editArray):
            temp = []
            for subloop,j in enumerate(i):
                if j not in ",":                        
                    temp.append(int(j))
            editor_data.append(temp)

        for loop,i in enumerate(editor_data):
            temp = []
            for subloop,j in enumerate(i):
                if j == 1:
                    newLot = ParkingLocation()
                    newLot.lotId = str(loop)+str(subloop)
                    newLot.zone = oldZone
                    newLot.save()

        oldZone.editor = editor_data
        oldZone.save()
        return redirect("/dashboard/")


def intro(request):
    camera = request.GET.get('camera',None)
    license = request.GET.get('license',None)
    print camera,license
    use = Vehicle.objects.all().filter(licensePlateNumber=license).values_list("caruser", flat=True)
    lot = PreferredParkingLocation.objects.all().filter(user=str(use[0])).values_list("preferredParkingLocation", flat=True)
    print use,lot
    for l in ParkingLocation.objects.all():
        if l.lotId == str(lot[0]):
            l.isParked = True
            l.save()
            return HttpResponse("Done")
    return HttpResponse("Error")