# Introduction #

Add your content here.


# Details #

Todos los elementos tienen una transformacion local y otra del mundo
los objetos van a estar en el sistema de coordenadas corespondiente a al escena
la posicion o tranformacion entre 2 objetos va a ser relativa a la escena
los objetos se relacionan en sistema de coordenadas al primer padre que tengan en comun
como se calcula la world en base a las locales:
> Local tanque (2,1)
> World Tanque(2,1)

> World Scene(0,0)
> Local Scene(0,0)

> Rueda World(1,1.5)
> Rueda Local(-1,0.5)

> Torreta World(2,2)
> Torreta local(0,1)

> Cañon World(3,2.5)
> Cañon Local(1,0.5)

World del padre mas la local de si mismo te da la world de si mismo
para convinarlas y que te de una sola matriz las multiplicas

World de un child = world del padre **Local del child
no puedo calcular la world de un objeto sin la world de su padre
Tengo que updatear y recalcular todas las worlds de la escena para poder
recalcular las locals y renderizar los objetos de nuevo.
SIEMPRE hay un root de donde parten los calculos.
para llamarlas en orden uso recursividad**


Nodo-->Parent
> -->Childs[n](n.md)

imprimir(Nodo n)
{
> print n.Name;
> for(c:Childs)
> {
> > imprimir(c);

> }
}

Se tiene que hacer desde que se carga la escena

Mesh(vertexbuffer y representacion visual)re implementa draw()-->Entity3D(transformaciones)(vuelve a node)va a tener draw() y updateTransformation()
Node()reimplementa draw() y updateTransformation()-->
patron composite



tambien hay que agregar setParent()
aplicar local matrix a update local transformation
si tiene parent sacamos la world entre la world del partent y la local y si no tiene es igual a la local
Al mesh le agregamos la reimplementacion del virtual de entity3D
Cuando seteamos la matrix que le damos al renderer le damos la world


Nodo solo tiene vector de childs, addchild() y removechild() ,draw de los childs y un updatetransformation()