import math

R_EARTH = 6371008.7714

def getradian(degree):
    radian = degree * math.pi / 180.0
    return radian

def getdegree(radian):
    degree = radian * 180.0 / math.pi
    return degree

def haversine(pos1, pos2):
	#radians zone (deg * pi/180)
	phi1 = getradian(pos1[0])
	phi2 = getradian(pos2[0])
	lambda1 = getradian(pos1[1])
	lambda2 = getradian(pos2[1])
	dlambda = lambda2 - lambda1
	dphi = phi2 - phi1;
	
	return pow(math.sin(dphi/2), 2) + math.cos(phi1) * math.cos(phi2) * pow(math.sin(dlambda/2),2);

def gpsbearing(pos1, pos2):
    phi1 = getradian(pos1[0])
    phi2 = getradian(pos2[0])
    lambda1 = getradian(pos1[1])
    lambda2 = getradian(pos2[1])

    dlambda = lambda2 - lambda1

    y = math.sin(dlambda) * math.cos(phi2)
    x = math.cos(phi1) * math.sin(phi2) - math.sin(phi1) * math.cos(phi2) * math.cos(dlambda)

    theta = math.atan2(y,x)

    #degrees zone (rad * 180/pi)
    bearing = getdegree(theta)
    if (bearing > 360.0):
        bearing = bearing - 360.0
    elif (bearing <= 0.0):
        bearing = bearing + 360.0
    return bearing

def gpsdistance(pos1, pos2):
	hav = haversine(pos1, pos2)
	arclength = 2 * math.atan2(math.sqrt(hav),math.sqrt(1-hav))
	distance = R_EARTH * arclength
	return distance
