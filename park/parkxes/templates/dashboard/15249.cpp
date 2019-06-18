
#include <stdio.h>

int main() {
    float tempInWest, tempInEast, tempInNorth, tempInSouth, areaCntrlVolume, thrmlConduct, l, br, g, h, x, heatValue, y, as, fluxInWest, fluxInEast, fluxInNorth, fluxInSouth, residue, trigger = 0;
    int nodesInX, nodesInY, i, j, w, z;
    printf("enter the no of nodes in Y direction");
    scanf("%d",&nodesInY);
    printf("enter the no of nodes in X direction");
    scanf("%d",&nodesInX);
    printf("enter the boundary conditions");
    printf("Temp in the west");
    scanf("%f",&tempInWest);
    printf("Heat flux in west (J/m^2)(if temperature is given enter zero)");
    scanf("%f",&fluxInWest);
    printf("Temp in the east");
    scanf("%f",&tempInEast);
    printf("Heat flux in east (J/m^2)(if temperature is given enter zero)");
    scanf("%f",&fluxInEast);
    printf("Temp in the north");
    scanf("%f",&tempInNorth);
    printf("Heat flux in north (J/m^2)(if temperature is given enter zero)");
    scanf("%f",&fluxInNorth);
    printf("Temp in the south");
    scanf("%f",&tempInSouth);
    printf("Heat flux in south (J/m^2)(if temperature is given enter zero)");
    scanf("%f",&fluxInSouth);
    printf("enter the value of heat source if any, else enter zero (J/m^3)");
    scanf("%f",&heatValue);
    printf("enter the area of the control volume");
    scanf("%f",&areaCntrlVolume);
    printf("enter the value of thermal conductivity");
    scanf("%f",&thrmlConduct);
    printf("enter the distance between the two ends");
    scanf("%f",&l);
    printf("enter the distance between the top and bottom");
    scanf("%f",&br);
    printf("enter the value of assumed conditions");
    scanf("%f",&as);
    int sol[nodesInY][nodesInX], temp[nodesInY][nodesInX];
    float a[nodesInX], c[nodesInX], b[nodesInX];
    float c1[nodesInX], r1[nodesInX];
    x = l / nodesInX;
    y = br / nodesInY;
    g = thrmlConduct * areaCntrlVolume / (x);
    h = thrmlConduct * areaCntrlVolume / (y);
    float T[nodesInX][nodesInX], r[nodesInX];
    for (i = 0; i < nodesInX; i++) {
        for (j = 0; j < nodesInX; j++) {
            T[i][j] = 0;
        }
    }
    for (i = 0; i < nodesInY; i++)
        for (j = 0; j < nodesInX; j++) {
            sol[i][j] = as;
        }
    for (i = 0; i < nodesInY; i++)
        for (j = 0; j < nodesInX; j++) {
            temp[i][j] = sol[i][j];
        }
    
    while (trigger != (nodesInY * nodesInX)) {
        trigger = 0;
        //first bar
        if ((fluxInWest == 0) && (fluxInNorth == 0)) {
            T[0][0] = 3 * g + 3 * h;
            T[0][1] = -1 * g;
        }
        if ((fluxInWest != 0) && (fluxInNorth != 0)) {
            T[0][0] = g + h;
            T[0][1] = -1 * g;
        }
        if ((fluxInWest == 0) && (fluxInNorth != 0)) {
            T[0][0] = 3 * g + h;
            T[0][1] = -1 * g;
        }
        if ((fluxInWest == 0) && (fluxInNorth != 0)) {
            T[0][0] = g + 3 * h;
            T[0][1] = -1 * g;
        }
        for (i = 1; i < nodesInX - 1; i++) {
            if (fluxInNorth == 0) {
                T[i][i] = 2 * g + (3 * h);
                T[i][i - 1] = -1 * g;
                T[i][i + 1] = -1 * g;
            }
            if (fluxInNorth != 0) {
                T[i][i] = 2 * g + (h);
                T[i][i - 1] = -1 * g;
                T[i][i + 1] = -1 * g;
            }
        }
        if ((fluxInNorth == 0) && (fluxInEast == 0)) {
            T[nodesInX - 1][nodesInX - 1] = 3 * g + 3 * h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        if ((fluxInNorth != 0) && (fluxInEast != 0)) {
            T[nodesInX - 1][nodesInX - 1] = g + h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        if ((fluxInNorth != 0) && (fluxInEast == 0)) {
            T[nodesInX - 1][nodesInX - 1] = 3 * g + h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        if ((fluxInNorth == 0) && (fluxInEast != 0)) {
            T[nodesInX - 1][nodesInX - 1] = g + 3 * h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        //end of entries of temp matrix
        for (i = 1; i < nodesInX - 1; i++) {
            if (fluxInNorth == 0) {
                r[i] = heatValue * areaCntrlVolume * x + (h * sol[1][i]) + (2 * h * tempInNorth);
            }
            if (fluxInNorth != 0) {
                r[i] = heatValue * areaCntrlVolume * x + (h * sol[1][i]) - fluxInNorth * areaCntrlVolume;
            }
        }
        if ((fluxInNorth == 0) && (fluxInWest == 0)) {
            r[0] = (2 * g * tempInWest) + (heatValue * areaCntrlVolume * x) + (h * sol[1][0]) + (2 * h * tempInNorth);
        }
        if ((fluxInNorth != 0) && (fluxInWest != 0)) {
            r[0] = (heatValue * areaCntrlVolume * x) + (h * sol[1][0]) - fluxInWest * areaCntrlVolume - fluxInNorth * areaCntrlVolume;
        }
        if ((fluxInNorth != 0) && (fluxInWest == 0)) {
            r[0] = (heatValue * areaCntrlVolume * x) + (h * sol[1][0]) - fluxInNorth * areaCntrlVolume + (2 * h * tempInNorth);
        }
        if ((fluxInNorth != 0) && (fluxInWest != 0)) {
            r[0] = (heatValue * areaCntrlVolume * x) + (h * sol[1][0]) - fluxInWest * areaCntrlVolume + (2 * h * tempInNorth);
        }
        //end r 1st node
        if ((fluxInNorth == 0) && (fluxInEast == 0)) {
            r[nodesInX - 1] = (2 * g * tempInEast) + (heatValue * areaCntrlVolume * x) + (2 * h * tempInNorth) + (h * sol[1][nodesInX - 1]);
        }
        if ((fluxInNorth != 0) && (fluxInEast != 0)) {
            r[nodesInX - 1] = -(fluxInNorth * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (fluxInEast * areaCntrlVolume) + (h * sol[1][nodesInX - 1]);
        }
        if ((fluxInNorth != 0) && (fluxInEast == 0)) {
            r[nodesInX - 1] = (2 * g * tempInEast) + (heatValue * areaCntrlVolume * x) - (fluxInNorth * areaCntrlVolume) + (h * sol[1][nodesInX - 1]);
        }
        if ((fluxInNorth == 0) && (fluxInEast != 0)) {
            r[nodesInX - 1] = (fluxInEast * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (2 * h * tempInNorth) + (h * sol[1][nodesInX - 1]);
        }
        //tridiagonal for first bar first iteration
        a[0] = 0;
        for (i = 1, j = 0; i < nodesInX, j < nodesInX - 1; i++, j++) {
            a[i] = T[i][j];
            
        }
        
        c[nodesInX - 1] = 0;
        for (i = 0, j = 1; i < nodesInX - 1, j < nodesInX; i++, j++) {
            c[i] = T[i][j];
            
        }
        
        for (i = 0; i < nodesInX; i++) {
            b[i] = T[i][i];
            
        }
        
        
        c1[0] = c[0] / b[0];
        for (i = 1; i < nodesInX; i++) {
            c1[i] = c[i] / (b[i] - (a[i] * c1[i - 1]));
            //cout<<c1[i];
        }
        
        r1[0] = r[0] / b[0];
        for (i = 1; i < nodesInX; i++) {
            r1[i] = (r[i] - (a[i] * r1[i - 1])) / (b[i] - (a[i] * c1[i - 1]));
            //cout<<r1[i];
        }
        
        sol[0][nodesInX - 1] = r1[nodesInX - 1];
        for (i = nodesInX - 2; i >= 0; i--) {
            sol[0][i] = r1[i] - (c1[i] * sol[0][i + 1]);
        }
        
        for (i = 0; i < nodesInX; i++) {
            //cout<<sol[0][i]<<"\n";
        }
        
        for (z = 1; z < nodesInY - 1; z++) {
            if (fluxInWest == 0) {
                T[0][0] = 3 * g + 2 * h;
                T[0][1] = -1 * g;
            }
            if (fluxInWest != 0) {
                T[0][0] = g + 2 * h;
                T[0][1] = -1 * g;
            }
            for (i = 1; i < nodesInX - 1; i++) {
                T[i][i] = 2 * g + (2 * h);
                T[i][i - 1] = -1 * g;
                T[i][i + 1] = -1 * g;
            }
            if (fluxInEast == 0) {
                T[nodesInX - 1][nodesInX - 1] = 3 * g + 2 * h;
                T[nodesInX - 1][nodesInX - 2] = -1 * g;
            }
            if (fluxInEast != 0) {
                T[nodesInX - 1][nodesInX - 1] = g + 2 * h;
                T[nodesInX - 1][nodesInX - 2] = -1 * g;
            }
            for (i = 1; i < nodesInX - 1; i++) {
                r[i] = heatValue * areaCntrlVolume * x + (h * sol[z - 1][i]) + (h * sol[z + 1][i]);
            }
            if (fluxInWest == 0) {
                r[0] = (2 * g * tempInWest) + (heatValue * areaCntrlVolume * x) + (h * sol[z + 1][0]) + (h * sol[z - 1][0]);
            }
            if (fluxInWest != 0) {
                r[0] = (-fluxInWest * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (h * sol[z + 1][0]) + (h * sol[z - 1][0]);
            }
            if (fluxInEast == 0) {
                r[nodesInX - 1] = (2 * g * tempInEast) + (heatValue * areaCntrlVolume * x) + (h * sol[z + 1][nodesInX - 1]) + (h * sol[z - 1][nodesInX - 1]);
            }
            if (fluxInEast != 0) {
                r[nodesInX - 1] = (fluxInEast * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (h * sol[z + 1][nodesInX - 1]) + (h * sol[z - 1][nodesInX - 1]);
            }
            
            //tridiagonal for zth bar unknown iteration
            a[0] = 0;
            for (i = 1, j = 0; i < nodesInX, j < nodesInX - 1; i++, j++) {
                a[i] = T[i][j];
                // cout<<a[i]<<"\t";
            }
            
            c[nodesInX - 1] = 0;
            for (i = 0, j = 1; i < nodesInX - 1, j < nodesInX; i++, j++) {
                c[i] = T[i][j];
                //cout<<c[i]<<"\t";
            }
            
            for (i = 0; i < nodesInX; i++) {
                b[i] = T[i][i];
                //       cout<<b[i]<<"\t";
            }
            
            
            c1[0] = c[0] / b[0];
            for (i = 1; i < nodesInX; i++) {
                c1[i] = c[i] / (b[i] - (a[i] * c1[i - 1]));
            }
            
            r1[0] = r[0] / b[0];
            for (i = 1; i < nodesInX; i++) {
                r1[i] = (r[i] - (a[i] * r1[i - 1])) / (b[i] - (a[i] * c1[i - 1]));
            }
            
            sol[z][nodesInX - 1] = r1[nodesInX - 1];
            for (i = nodesInX - 2; i >= 0; i--) {
                sol[z][i] = r1[i] - (c1[i] * sol[z][i + 1]);
            }
            
            for (i = 0; i < nodesInX; i++) {
                //cout<<sol[z][i]<<"\n";
            }
        }
        //last bar
        if ((fluxInWest == 0) && (fluxInSouth == 0)) {
            T[0][0] = 3 * g + 3 * h;
            T[0][1] = -1 * g;
        }
        if ((fluxInWest != 0) && (fluxInSouth != 0)) {
            T[0][0] = g + h;
            T[0][1] = -1 * g;
        }
        if ((fluxInWest != 0) && (fluxInSouth == 0)) {
            T[0][0] = g + 3 * h;
            T[0][1] = -1 * g;
        }
        if ((fluxInWest == 0) && (fluxInSouth != 0)) {
            T[0][0] = 3 * g + h;
            T[0][1] = -1 * g;
        }
        
        for (i = 1; i < nodesInX - 1; i++) {
            if (fluxInSouth == 0) {
                T[i][i] = 2 * g + (3 * h);
                T[i][i - 1] = -1 * g;
                T[i][i + 1] = -1 * g;
            }
            if (fluxInSouth != 0) {
                T[i][i] = 2 * g + (h);
                T[i][i - 1] = -1 * g;
                T[i][i + 1] = -1 * g;
            }
        }
        if ((fluxInEast == 0) && (fluxInSouth == 0)) {
            T[nodesInX - 1][nodesInX - 1] = 3 * g + 3 * h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        if ((fluxInEast != 0) && (fluxInSouth != 0)) {
            T[nodesInX - 1][nodesInX - 1] = g + h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        if ((fluxInEast != 0) && (fluxInSouth == 0)) {
            T[nodesInX - 1][nodesInX - 1] = g + 3 * h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        if ((fluxInEast == 0) && (fluxInSouth != 0)) {
            T[nodesInX - 1][nodesInX - 1] = 3 * g + h;
            T[nodesInX - 1][nodesInX - 2] = -1 * g;
        }
        for (i = 1; i < nodesInX - 1; i++) {
            if (fluxInSouth == 0) {
                r[i] = heatValue * areaCntrlVolume * x + (h * sol[nodesInY - 2][i]) + (2 * h * tempInSouth);
            }
            if (fluxInSouth != 0) {
                r[i] = heatValue * areaCntrlVolume * x + (h * sol[nodesInY - 2][i]) + (fluxInSouth * areaCntrlVolume);
            }
        }
        if ((fluxInSouth == 0) && (fluxInWest == 0))
            r[0] = (2 * g * tempInWest) + (heatValue * areaCntrlVolume * x) + (h * sol[nodesInY - 2][0]) + (2 * h * tempInSouth);
        if ((fluxInSouth != 0) && (fluxInWest != 0))
            r[0] = (fluxInSouth * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (h * sol[nodesInY - 2][0]) - (fluxInWest * areaCntrlVolume);
        if ((fluxInSouth != 0) && (fluxInWest == 0))
            r[0] = (2 * g * tempInWest) + (heatValue * areaCntrlVolume * x) + (h * sol[nodesInY - 2][0]) + (fluxInSouth * areaCntrlVolume);
        if ((fluxInSouth == 0) && (fluxInWest != 0))
            r[0] = -(fluxInWest * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (h * sol[nodesInY - 2][0]) + (2 * h * tempInSouth);
        
        if ((fluxInSouth == 0) && (fluxInEast == 0))
            r[nodesInX - 1] = (2 * g * tempInEast) + (heatValue * areaCntrlVolume * x) + (2 * h * tempInSouth) + (h * sol[nodesInY - 2][nodesInX - 1]);
        if ((fluxInSouth != 0) && (fluxInEast != 0))
            r[nodesInX - 1] = (fluxInSouth * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (fluxInEast * areaCntrlVolume) + (h * sol[nodesInY - 2][nodesInX - 1]);
        if ((fluxInSouth != 0) && (fluxInEast == 0))
            r[nodesInX - 1] = (2 * g * tempInEast) + (heatValue * areaCntrlVolume * x) + (fluxInSouth * areaCntrlVolume) + (h * sol[nodesInY - 2][nodesInX - 1]);
        if ((fluxInSouth == 0) && (fluxInEast != 0))
            r[nodesInX - 1] = (fluxInEast * areaCntrlVolume) + (heatValue * areaCntrlVolume * x) + (2 * h * tempInSouth) + (h * sol[nodesInY - 2][nodesInX - 1]);
        
        //tridiagonal for last bar first iteration
        a[0] = 0;
        for (i = 1, j = 0; i < nodesInX, j < nodesInX - 1; i++, j++) {
            a[i] = T[i][j];
            //       cout<<a[i]<<"\t";
        }
        
        c[nodesInX - 1] = 0;
        for (i = 0, j = 1; i < nodesInX - 1, j < nodesInX; i++, j++) {
            c[i] = T[i][j];
            //   cout<<c[i]<<"\t";
        }
        
        for (i = 0; i < nodesInX; i++) {
            b[i] = T[i][i];
            //cout<<b[i]<<"\t";
        }
        
        
        c1[0] = c[0] / b[0];
        for (i = 1; i < nodesInX; i++) {
            c1[i] = c[i] / (b[i] - (a[i] * c1[i - 1]));
        }
        
        r1[0] = r[0] / b[0];
        for (i = 1; i < nodesInX; i++) {
            r1[i] = (r[i] - (a[i] * r1[i - 1])) / (b[i] - (a[i] * c1[i - 1]));
        }
        
        sol[nodesInY - 1][nodesInX - 1] = r1[nodesInX - 1];
        for (i = nodesInX - 2; i >= 0; i--) {
            sol[nodesInY - 1][i] = r1[i] - (c1[i] * sol[nodesInY - 1][i + 1]);
        }
        
        for (i = 0; i < nodesInX; i++) {
            //cout<<sol[m-1][i]<<"\n";
        }
        
        for (i = 0; i < nodesInY; i++) {
            for (j = 0; j < nodesInX; j++) {
                residue = temp[i][j] - sol[i][j];
                if (residue < (0.0001) && residue > (-0.0001)) {
                    trigger = trigger + 1;
                }
            }
            
            //end of the while loop
        }
        for (i = 0; i < nodesInY; i++)
            for (j = 0; j < nodesInX; j++) {
                temp[i][j] = sol[i][j];
            }
        
        for (i = 0; i < nodesInY; i++)
            for (j = 0; j < nodesInX; j++) {
                
            }
    }
    
    for (i = 0; i < nodesInY; i++) {
        for (j = 0; j < nodesInX; j++) {
            printf("%f \t",(float)sol[i][j]);
        }
        printf("\n");
    }
    return 0;
}
