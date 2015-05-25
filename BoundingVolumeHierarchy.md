Bounding Volume Hierarchy (BVH)

bouding volume: un volumen en el cual el mesh cae totalmente dentro de ese volumen (caja,circunferencia,capsula,etc)
En la jerarquia de bounding volumes, cada bounding volume contiene a los childs de cada mesh,

1->2
> ->3

2 y 3 tendrian cada uno su bounding box, a su vez ambos estarian contenidos en otro y esos 2 + 1 estarian en otro mas grande
Si el volumen de la camara colisiona con el volumen del nodo significa que hay visibilidad
Axis aligned bounding box (sin rotacion)

Se usa a nivel de entity porque tanto mesh como nodos aplican para el algoritmo
Hay que ver si el mesh colisiona con el frustum



Mesh::checkCollision(Frustum F)
{
> isVisible = aabb.checkCollision(F);
}


Node::checkCollision(Frustum F)
{
> Result r=aabb.checkCollision(F);
> if(r = Parcial)
> {
> > for(c:Child)
> > {
> > > c.checkCollision(F);

> > }
> > isVisible = (r != noCollision);

> }
}

Esto es para saber que se debe dibujar y que no, solo mando al draw aquello que colisiona

El AABB lo calculo tomando la diferencia entre el mayor y el menor vertice del mesh en cada eje del mundo, al restarlos
me va a dar el valor en  x,y y z del bounding box
Para los nodos con el AABB y la world saco los maximos y minimos de la caja que encierra a los nodos

Frustum vs AABB
Para saber si esta dentro del frustum veo si el punto esta del lado de las normales de los planos que forman el frustum

El algoritmo para calcular las AABBs se repite todos los frames.