// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>

struct Point{
    float x;
    float y;
};

struct Triangle {
    int id;
    Point a, b, c;
    int ab, bc, ca;
};

// Funzione per determinare l'orientamento di tre punti (in senso orario, antiorario o allineati)
int orientation(const Point& p1, const Point& p2, const Point& p3) {
    int val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);
    if (val == 0) return 0;  // Allineati
    return (val > 0) ? 1 : -1;  // Antiorario o orario
}

// Funzione per controllare se un punto è all'interno di un triangolo
bool isInsideTriangle(const Point& p, const Triangle& t) {
    Point p1, p2, p3;
    p1 = t.a;
    p2 = t.b;
    p3 = t.c;

    int d1, d2, d3;
    d1 = orientation(p, p1, p2);
    d2 = orientation(p, p2, p3);
    d3 = orientation(p, p3, p1);

    return (d1 == d2 && d2 == d3);
}

// Funzione ricorsiva per flipping dei triangoli adiacenti
void performFlipping(std::vector<Triangle>& mesh, int currentTriangleIndex, int sharedEdgeIndex) {
    Triangle& currentTriangle = mesh[currentTriangleIndex];
    int nextTriangleIndex = currentTriangle.ab;  // Prendi il triangolo adiacente sull'edge AB

    if (nextTriangleIndex == -1) return;  // Nessun adiacente, termina la ricorsione

    Triangle& nextTriangle = mesh[nextTriangleIndex];

    // Trova il punto condiviso sull'edge dei triangoli correnti
    Point sharedPoint1, sharedPoint2;
    if (sharedEdgeIndex == 0) {
        sharedPoint1 = currentTriangle.a;
        sharedPoint2 = currentTriangle.b;
    } else if (sharedEdgeIndex == 1) {
        sharedPoint1 = currentTriangle.b;
        sharedPoint2 = currentTriangle.c;
    } else if (sharedEdgeIndex == 2) {
        sharedPoint1 = currentTriangle.c;
        sharedPoint2 = currentTriangle.a;
    }

    // Trova l'indice dell'edge condiviso nell'adiacente
    int sharedEdgeIndexNextTriangle;
    if (nextTriangle.ab == currentTriangleIndex) {
        sharedEdgeIndexNextTriangle = 0;
    } else if (nextTriangle.bc == currentTriangleIndex) {
        sharedEdgeIndexNextTriangle = 1;
    } else if (nextTriangle.ca == currentTriangleIndex) {
        sharedEdgeIndexNextTriangle = 2;
    }

    // Controlla se il flipping è necessario
    Point sharedPoint = sharedPoint1;
    Point p1 = sharedPoint2;
    Point p2, p3;
    if (sharedEdgeIndexNextTriangle == 0) {
        p2 = nextTriangle.b;
        p3 = nextTriangle.c;
    } else if (sharedEdgeIndexNextTriangle == 1) {
        p2 = nextTriangle.c;
        p3 = nextTriangle.a;
    } else if (sharedEdgeIndexNextTriangle == 2) {
        p2 = nextTriangle.a;
        p3 = nextTriangle.b;
    }

    if (orientation(sharedPoint, p1, p2) < 0 && orientation(sharedPoint, p1, p3) < 0) {
        // Effettua il flipping
        // Aggiorna i riferimenti degli edge
        int oldAB = currentTriangle.ab;
        int oldBC = currentTriangle.bc;
        int oldCA = currentTriangle.ca;

        currentTriangle.ab = nextTriangleIndex;
        currentTriangle.bc = sharedEdgeIndexNextTriangle;

        if (sharedEdgeIndex == 0) {
            currentTriangle.ca = nextTriangle.ca;
            nextTriangle.ca = currentTriangleIndex;
            nextTriangle.ab = oldAB;
        } else if (sharedEdgeIndex == 1) {
            currentTriangle.ca = nextTriangle.ab;
            nextTriangle.ab = currentTriangleIndex;
            nextTriangle.bc = oldBC;
        } else if (sharedEdgeIndex == 2) {
            currentTriangle.ca = nextTriangle.bc;
            nextTriangle.bc = currentTriangleIndex;
            nextTriangle.ca = oldCA;
        }

        // Richiama la funzione ricorsivamente per il prossimo flipping
        performFlipping(mesh, currentTriangleIndex, sharedEdgeIndex);
    }
}

// Funzione per creare un nuovo triangolo dato un punto interno
void createNewTriangle(std::vector<Triangle>& mesh, Point point, int triangleIndex, int sharedEdgeIndex) {
    int newTriangleIndex = mesh.size();  // Indice del nuovo triangolo
    Triangle newTriangle;
    newTriangle.id = newTriangleIndex;
    newTriangle.a = point;

    Triangle& currentTriangle = mesh[triangleIndex];
    int nextTriangleIndex = currentTriangle.ab;  // Prendi il triangolo adiacente sull'edge AB

    if (nextTriangleIndex == -1) {
        // Il triangolo corrente non ha un adiacente sull'edge AB
        newTriangle.b = currentTriangle.b;
        newTriangle.c = currentTriangle.a;

        currentTriangle.ab = newTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul primo triangolo

        // Crea l'adiacente per il nuovo triangolo
        Triangle newAdjacent;
        newAdjacent.id = triangleIndex;
        newAdjacent.a = point;
        newAdjacent.b = currentTriangle.a;
        newAdjacent.c = currentTriangle.c;

        newAdjacent.bc = newTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul secondo triangolo
        mesh.push_back(newAdjacent);
    } else {
        // Il triangolo corrente ha un adiacente sull'edge AB
        Triangle& nextTriangle = mesh[nextTriangleIndex];

        newTriangle.b = currentTriangle.b;
        newTriangle.c = nextTriangle.b;

        currentTriangle.ab = newTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul primo triangolo
        nextTriangle.ca = newTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul secondo triangolo

        // Crea gli adiacenti per i nuovi triangoli
        Triangle newAdjacent1;
        newAdjacent1.id = triangleIndex;
        newAdjacent1.a = point;
        newAdjacent1.b = currentTriangle.a;
        newAdjacent1.c = currentTriangle.c;

        newAdjacent1.bc = newTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul primo triangolo
        newAdjacent1.ca = nextTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul secondo triangolo

        Triangle newAdjacent2;
        newAdjacent2.id = newTriangleIndex;
        newAdjacent2.a = point;
        newAdjacent2.b = nextTriangle.a;
        newAdjacent2.c = nextTriangle.c;

        newAdjacent2.bc = triangleIndex;  // Aggiorna il riferimento dell'adiacente sul primo triangolo
        newAdjacent2.ca = newTriangleIndex;  // Aggiorna il riferimento dell'adiacente sul secondo triangolo

        mesh.push_back(newAdjacent1);
        mesh.push_back(newAdjacent2);
    }

    mesh.push_back(newTriangle);  // Aggiungi il nuovo triangolo alla mesh
}

// Funzione per creare un nuovo triangolo dato un punto esterno
void createTriangleFromExternalPoint(std::vector<Triangle>& mesh, const Point& externalPoint) {
    int n = mesh.size();
    for (int i = 0; i < n; i++) {
        Triangle& currentTriangle = mesh[i];
        Point p1, p2, p3;
        p1 = currentTriangle.a;
        p2 = currentTriangle.b;
        p3 = currentTriangle.c;

        int d1, d2, d3;
        d1 = orientation(externalPoint, p1, p2);
        d2 = orientation(externalPoint, p2, p3);
        d3 = orientation(externalPoint, p3, p1);

        if (d1 >= 0 && d2 >= 0 && d3 >= 0) {
            // Il punto esterno si trova all'interno di questo triangolo
            createNewTriangle(mesh, externalPoint, i, -1);  // -1 indica che il punto è all'interno del triangolo
            return;
        }
    }

    // Il punto esterno non si trova all'interno di nessun triangolo, quindi crea un nuovo triangolo con il punto come vertice
    Triangle newTriangle;
    newTriangle.id = n;
    newTriangle.a = externalPoint;
    //newTriangle.b = (externalPoint + 1) % n;
    //newTriangle.c = (externalPoint + 2) % n;
    //newTriangle.ab = newTriangle.bc = newTriangle.ca = -1;  // Nessun adiacente

    mesh.push_back(newTriangle);
}

int main() {
    std::vector<Triangle> mesh;
    Point listP[1000];

    // Popola la mesh con i triangoli seguendo il criterio di Delaunay

    // Esempio di utilizzo delle funzioni:

    // Flipping
    performFlipping(mesh, 0, 0);  // Esegui il flipping sul primo triangolo, 

    // Creazione di un nuovo triangolo con un punto interno
    Point internalPoint;
    //internalPoint.id = mesh.size();  // Assegna un nuovo ID al punto
    internalPoint.x = 1.0;
    internalPoint.y = 1.0;


    createNewTriangle(mesh, internalPoint, 0, 0);  // Crea un nuovo 

    // Creazione di un nuovo triangolo con un punto esterno
    Point externalPoint;
   
    externalPoint.x = 3.0;
    externalPoint.y = 2.0;
    

    createTriangleFromExternalPoint(mesh, externalPoint);  // Crea un nuovo triangolo con il punto esterno

    return 0;
}