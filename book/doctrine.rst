.. index::
   single: Doctrine

Veritabanları ve Doctrine
==========================
Şunu kabul edelim. Herhangi bir uygulamanın en zorlu işleri veritabanından
bilgileri okumak ya da buraya bilgleri yazmaktır. Çok şükür, Symfony 
`Doctrine`_ adındaki bu işleri kolaylıkla yapacağınız araçları içeren bir
kütüphane ile birlikte entegre gelir. Bu kısımda Doctrine'ın arkasındaki
temel felsefeyi öğrenecek ve veritabanları ile çalışmanın ne kadar kolay
olabileceğini göreceksiniz. 

.. note::

    Doctrine isteğe bağlı olarak kullanabilmeniz için Symfony2'den tamamen 
    ayrıştırılmıştır. Bu kısım tamamen Doctrine ORM ile nesnelerinizi ilişkisel
    veri tabanında  (*MySQL*, *PostgreSQL* ya da  *Microsoft SQL*) nasıl 
    kullanacağınızı içerir.
    Eğer saf veritabanı cümlecikleri kullanmak isterseniz bu da basittir 
    ve ":doc:`/cookbook/doctrine/dbal`" tarif kitabı girdisinde açıklanmıştır.
    Ayrıca verilerinizi  Doctrine ODM kullanarak da `MongoDB`_ üzerinde
    işleyebilirsiniz. Bu konudaki daha fazla bilgi için 
    ":doc:`/bundles/DoctrineMongoDBBundle/index`" belgesini okuyabilirsiniz.

Basit Bir Örnek: Bir Ürün (Product)
-----------------------------------

Doctrine'nin nasıl çalıştığını anlamanın en iyi yolu onu uygulamada görmektir.
Bu kısımda veritabanını konfigüre edecek ``Product`` nesnesi yaratacak
bunu veri tabanına yazacak ve veritabanından geri çağıracaksınız.

.. sidebar:: Kodu örnek üzerinde takip etmek

    Eğer bu kısmı bir örnekle birlikte takip etmek istiyorsanız,
    şu komut satırı komutu ile ``AcmeStoreBundle`` bundle'ını yaratın:
    
    .. code-block:: bash
    
        php app/console generate:bundle --namespace=Acme/StoreBundle

Veritabanını Konfigüre Etmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Gerçekten başlamadan önce veritabanı bağlantı bilgisini konfigüre etmek
gerekli. Kural olarak bu bilgi genellikle ``app/config/parameters.ini``
dosyasında tutulur:

.. code-block:: ini

    ;app/config/parameters.ini
    [parameters]
        database_driver   = pdo_mysql
        database_host     = localhost
        database_name     = test_project
        database_user     = root
        database_password = password

.. note::

   ``parameters.ini`` dosyası ile konfigüre etmek işi sadece bir teamüldür.
   Parametreler ana konfigürasyon dosyasında doctrine altında gösterilerek de 
   ayarlanabilir.
   
    
    .. code-block:: yaml
    
        doctrine:
            dbal:
                driver:   %database_driver%
                host:     %database_host%
                dbname:   %database_name%
                user:     %database_user%
                password: %database_password%
    
    
    Veritabanı bilgisinin farklı bir dosya da ayrılması ile her sunucu
    için bu dosyanın farklı şekillerini tutmanız kolaylaşır.
    Ayrıca kolaylıkla veritabanı konfigürasyonunu (ya da önemli herhangi
    bir veriyi) projenizden ,mesela apache konfigürasyonu gibi,
    ayrımış olursunuz. Daha fazla bilgi için 
    :doc:`/cookbook/configuration/external_parameters` belgesini okuyun.


Bundan sonra Doctine veritabanınızı tanıyor ve şu şekilde de sizin için bir
veritabanı yaratabilir:

.. code-block:: bash

    php app/console doctrine:database:create

Bir Entity (Varlık) Sınıfı Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Varsayalım ürünleri listeleyen bir uygulama geliştiriyorsunuz. Doctrine yada
veritabanları olmadan düşünseniz bile zaten bildiğiniz gibi tüm ürünleri
temsil eden bir ``Product`` nesnesine ihtiyacınızın olduğudur.
Bu sınıfı ``AcmeStoreBundle`` içerisindeki ``Entity`` klasörü altında 
yaratın:: 

    // src/Acme/StoreBundle/Entity/Product.php    
    namespace Acme\StoreBundle\Entity;

    class Product
    {
        protected $name;

        protected $price;

        protected $description;
    }

Bu sınıf - sıklıkla "entity" adıyla adlandırılan bu sınıfın anlamı *verileri 
tutan temel sınıftır* - basittir ve uygulamanızda işletmenizin ihtiyacı 
olan tüm ürünlerin listelenmesi işine yardım eder. Bu sınıf henüz veritabanına
kayıt yapamamaktadır. Bu sadece basit bir PHP sınıfıdır.

.. tip::

    Doctrine bu entity sınıfını sizin için aşağıdaki komut ile yaratabilir
    ancak öncelikle Doctrine'nin arkasındaki konseptleri öğrenin:
    
    .. code-block:: bash
        
        php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Product" --fields="name:string(255) price:float description:text"

.. index::
    single: Doctrine; metadata eşleştirmesi eklemek (mapping metadata)

.. _book-doctrine-adding-mapping:

Eşleme Bilgisi Ekleme (Mapping Information)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Doctrine size veritabanları ile çalışırken sadece sütün tabanlı verileri
tablodan bir dize değişkenine çekmekten çok daha ilginç bir şekilde çalışmanıza
izin verir. Verileri dize değişkenine aktarmak yerine, Doctrine tüm *nesneleri*
veritabanına yazmaya ve tüm nesneleri veritabanından çekmeye izin verir. 
Bu PHP sınıfının veritabanı tablosu ile eşleştirilmesi (mapping) ile olur ve
PHP sınıfının değişkenleri tablonun sütünları haline gelir:

.. image:: /images/book/doctrine_image_1.png
   :align: center

Doctrinde bunu yapabilmek için öncelikle bir "metadata" yaratmak ya da 
Doctrine'e tam olarak ``Product`` sınıfının ve değişkenlerinin veritabanında
nasıl *eşleşeceğini* (map) düzenleyecek şekilde konfigüre etmeniz gerekir.
Bu metadata YAML, XML gibi formatlarda da olacağı gibi ``Product`` sınıfı
içerisinde belirteçler (annotation) aracılığı ile de olabilir:

.. note::

    Bir bundle sadece bir metadata tanımlama formatını tanıyabilir. Örneğin,
    YAML tipinde tutulan metadata bilgilerinin belirteçler ile PHP sınıfında da
    kullanılması gibi karışık bir kullanım mümkün değildir.


.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity
         * @ORM\Table(name="product")
         */
        class Product
        {
            /**
             * @ORM\Id
             * @ORM\Column(type="integer")
             * @ORM\GeneratedValue(strategy="AUTO")
             */
            protected $id;

            /**
             * @ORM\Column(type="string", length=100)
             */
            protected $name;

            /**
             * @ORM\Column(type="decimal", scale=2)
             */
            protected $price;

            /**
             * @ORM\Column(type="text")
             */
            protected $description;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            table: product
            id:
                id:
                    type: integer
                    generator: { strategy: AUTO }
            fields:
                name:
                    type: string
                    length: 100
                price:
                    type: decimal
                    scale: 2
                description:
                    type: text

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <doctrine-mapping xmlns="http://doctrine-project.org/schemas/orm/doctrine-mapping"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://doctrine-project.org/schemas/orm/doctrine-mapping
                            http://doctrine-project.org/schemas/orm/doctrine-mapping.xsd">

            <entity name="Acme\StoreBundle\Entity\Product" table="product">
                <id name="id" type="integer" column="id">
                    <generator strategy="AUTO" />
                </id>
                <field name="name" column="name" type="string" length="100" />
                <field name="price" column="price" type="decimal" scale="2" />
                <field name="description" column="description" type="text" />
            </entity>
        </doctrine-mapping>

.. tip::

    
    Tablo ismi seçimliktir ve eğer atlanırsa tablo adı otomatik olarak 
    entity sınıfının ismi olarak belirlenir.


Doctrine geniş bir yelpazede çeşitli alan tip tanımlamalarını her birisinin
kendi özel kullanımlarıyla tanıyabilir. Var olan alan tipleri için 
:ref:`book-doctrine-field-types` kısmına bakın.

.. seealso::

    
    Aynı zamanda  eşleme bilgisi hakkında tüm herşey için Doctrine'nin 
    `Basit Eşleme Belgesi`_  belgesine de bakabilirsiniz.
    Eğer belirteç (annotation) kullanıyorsanız bu alıntıların önlerine
    ``ORM\`` ifadsini eklemeniz gereklidir. (Örn. : ``ORM\Column(..)``)
    Bu Doctrine'nin kendi belgelerinde gösterilmemektedir. Ayrıca 
    ``use Doctrine\ORM\Mapping as ORM;`` ifadesini kullanarak ``ORM``
    belirteci ön ekini *kullanabilir* hale gelirsiniz.
    
.. caution::

    Sınıf isimlerinde ve sınıfın değişkenlerinde korunan SQL anahtar kelimeleri
    ni kullanmayınız(``group`` ya da ``user`` gibi).Bunlar eşleştirilmezler. 
    Örneğin eğer entity sınıfınızın adı ``Group`` ise varsayılan tablo adınızda
    ``group`` olacak ve SQL motorunda bu hataya neden olacaktır. Doctrine'nin
    `Ayrılmış SQL anahtar kelimeleri belgesi`_  'ni okuyarak bunlardan nasıl
    kaçınabileceğinizi öğrenebilirsiniz.
   

.. note::

    Belirteçleri kullanan başka bir kütüphane yada program kullandığınızda
    (örn: Doxygen) ``@IgnoreAnnotation`` belirtecini kullanarak Symfony' nin
    hangi belirteçleri görmemezden geleceğini ayarlamanız gereklidir.
    
    Örneğin, ``@fn`` belirtecinin bir hataya sebep olmasını engellemek için
    şu ifadeyi kullanmanız gerekmektedir::


        /**
         * @IgnoreAnnotation("fn")
         */
        class Product

Generating Getters and Setters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Even though Doctrine now knows how to persist a ``Product`` object to the
database, the class itself isn't really useful yet. Since ``Product`` is just
a regular PHP class, you need to create getter and setter methods (e.g. ``getName()``,
``setName()``) in order to access its properties (since the properties are
``protected``). Fortunately, Doctrine can do this for you by running:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product

This command makes sure that all of the getters and setters are generated
for the ``Product`` class. This is a safe command - you can run it over and
over again: it only generates getters and setters that don't exist (i.e. it
doesn't replace your existing methods).

.. sidebar:: More about ``doctrine:generate:entities``

    With the ``doctrine:generate:entities`` command you can:

        * generate getters and setters,

        * generate repository classes configured with the
            ``@ORM\Entity(repositoryClass="...")`` annotation,

        * generate the appropriate constructor for 1:n and n:m relations.

    The ``doctrine:generate:entities`` command saves a backup of the original
    ``Product.php`` named ``Product.php~``. In some cases, the presence of
    this file can cause a "Cannot redeclare class" error. It can be safely
    removed.

    Note that you don't *need* to use this command. Doctrine doesn't rely
    on code generation. Like with normal PHP classes, you just need to make
    sure that your protected/private properties have getter and setter methods.
    Since this is a common thing to do when using Doctrine, this command
    was created.

You can also generate all known entities (i.e. any PHP class with Doctrine
mapping information) of a bundle or an entire namespace:

.. code-block:: bash

    php app/console doctrine:generate:entities AcmeStoreBundle
    php app/console doctrine:generate:entities Acme

.. note::

    Doctrine doesn't care whether your properties are ``protected`` or ``private``,
    or whether or not you have a getter or setter function for a property.
    The getters and setters are generated here only because you'll need them
    to interact with your PHP object.

Creating the Database Tables/Schema
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You now have a usable ``Product`` class with mapping information so that
Doctrine knows exactly how to persist it. Of course, you don't yet have the
corresponding ``product`` table in your database. Fortunately, Doctrine can
automatically create all the database tables needed for every known entity
in your application. To do this, run:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    Actually, this command is incredibly powerful. It compares what
    your database *should* look like (based on the mapping information of
    your entities) with how it *actually* looks, and generates the SQL statements
    needed to *update* the database to where it should be. In other words, if you add
    a new property with mapping metadata to ``Product`` and run this task
    again, it will generate the "alter table" statement needed to add that
    new column to the existing ``product`` table.

    An even better way to take advantage of this functionality is via
    :doc:`migrations</bundles/DoctrineMigrationsBundle/index>`, which allow you to
    generate these SQL statements and store them in migration classes that
    can be run systematically on your production server in order to track
    and migrate your database schema safely and reliably.

Your database now has a fully-functional ``product`` table with columns that
match the metadata you've specified.

Persisting Objects to the Database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Now that you have a mapped ``Product`` entity and corresponding ``product``
table, you're ready to persist data to the database. From inside a controller,
this is pretty easy. Add the following method to the ``DefaultController``
of the bundle:

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...
    
    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice('19.99');
        $product->setDescription('Lorem ipsum dolor');

        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($product);
        $em->flush();

        return new Response('Created product id '.$product->getId());
    }

.. note::

    If you're following along with this example, you'll need to create a
    route that points to this action to see it work.

Let's walk through this example:

* **lines 8-11** In this section, you instantiate and work with the ``$product``
  object like any other, normal PHP object;

* **line 13** This line fetches Doctrine's *entity manager* object, which is
  responsible for handling the process of persisting and fetching objects
  to and from the database;

* **line 14** The ``persist()`` method tells Doctrine to "manage" the ``$product``
  object. This does not actually cause a query to be made to the database (yet).

* **line 15** When the ``flush()`` method is called, Doctrine looks through
  all of the objects that it's managing to see if they need to be persisted
  to the database. In this example, the ``$product`` object has not been
  persisted yet, so the entity manager executes an ``INSERT`` query and a
  row is created in the ``product`` table.

.. note::

  In fact, since Doctrine is aware of all your managed entities, when you
  call the ``flush()`` method, it calculates an overall changeset and executes
  the most efficient query/queries possible. For example, if you persist a
  total of 100 ``Product`` objects and then subsequently call ``flush()``, 
  Doctrine will create a *single* prepared statement and re-use it for each 
  insert. This pattern is called *Unit of Work*, and it's used because it's 
  fast and efficient.

When creating or updating objects, the workflow is always the same. In the
next section, you'll see how Doctrine is smart enough to automatically issue
an ``UPDATE`` query if the record already exists in the database.

.. tip::

    Doctrine provides a library that allows you to programmatically load testing
    data into your project (i.e. "fixture data"). For information, see
    :doc:`/bundles/DoctrineFixturesBundle/index`.

Fetching Objects from the Database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Fetching an object back out of the database is even easier. For example,
suppose you've configured a route to display a specific ``Product`` based
on its ``id`` value::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);
        
        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        // do something, like pass the $product object into a template
    }

When you query for a particular type of object, you always use what's known
as its "repository". You can think of a repository as a PHP class whose only
job is to help you fetch entities of a certain class. You can access the
repository object for an entity class via::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    The ``AcmeStoreBundle:Product`` string is a shortcut you can use anywhere
    in Doctrine instead of the full class name of the entity (i.e. ``Acme\StoreBundle\Entity\Product``).
    As long as your entity lives under the ``Entity`` namespace of your bundle,
    this will work.

Once you have your repository, you have access to all sorts of helpful methods::

    // query by the primary key (usually "id")
    $product = $repository->find($id);

    // dynamic method names to find based on a column value
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // find *all* products
    $products = $repository->findAll();

    // find a group of products based on an arbitrary column value
    $products = $repository->findByPrice(19.99);

.. note::

    Of course, you can also issue complex queries, which you'll learn more
    about in the :ref:`book-doctrine-queries` section.

You can also take advantage of the useful ``findBy`` and ``findOneBy`` methods
to easily fetch objects based on multiple conditions::

    // query for one product matching be name and price
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // query for all products matching the name, ordered by price
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

    When you render any page, you can see how many queries were made in the
    bottom right corner of the web debug toolbar.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    If you click the icon, the profiler will open, showing you the exact
    queries that were made.

Updating an Object
~~~~~~~~~~~~~~~~~~

Once you've fetched an object from Doctrine, updating it is easy. Suppose
you have a route that maps a product id to an update action in a controller::

    public function updateAction($id)
    {
        $em = $this->getDoctrine()->getEntityManager();
        $product = $em->getRepository('AcmeStoreBundle:Product')->find($id);

        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }

        $product->setName('New product name!');
        $em->flush();

        return $this->redirect($this->generateUrl('homepage'));
    }

Updating an object involves just three steps:

1. fetching the object from Doctrine;
2. modifying the object;
3. calling ``flush()`` on the entity manager

Notice that calling ``$em->persist($product)`` isn't necessary. Recall that
this method simply tells Doctrine to manage or "watch" the ``$product`` object.
In this case, since you fetched the ``$product`` object from Doctrine, it's
already managed.

Deleting an Object
~~~~~~~~~~~~~~~~~~

Deleting an object is very similar, but requires a call to the ``remove()``
method of the entity manager::

    $em->remove($product);
    $em->flush();

As you might expect, the ``remove()`` method notifies Doctrine that you'd
like to remove the given entity from the database. The actual ``DELETE`` query,
however, isn't actually executed until the ``flush()`` method is called.

.. _`book-doctrine-queries`:

Querying for Objects
--------------------

You've already seen how the repository object allows you to run basic queries
without any work::

    $repository->find($id);
    
    $repository->findOneByName('Foo');

Of course, Doctrine also allows you to write more complex queries using the
Doctrine Query Language (DQL). DQL is similar to SQL except that you should
imagine that you're querying for one or more objects of an entity class (e.g. ``Product``)
instead of querying for rows on a table (e.g. ``product``).

When querying in Doctrine, you have two options: writing pure Doctrine queries
or using Doctrine's Query Builder.

Querying for Objects with DQL
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Imagine that you want to query for products, but only return products that
cost more than ``19.99``, ordered from cheapest to most expensive. From inside
a controller, do the following::

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();

If you're comfortable with SQL, then DQL should feel very natural. The biggest
difference is that you need to think in terms of "objects" instead of rows
in a database. For this reason, you select *from* ``AcmeStoreBundle:Product``
and then alias it as ``p``.

The ``getResult()`` method returns an array of results. If you're querying
for just one object, you can use the ``getSingleResult()`` method instead::

    $product = $query->getSingleResult();

.. caution::

    The ``getSingleResult()`` method throws a ``Doctrine\ORM\NoResultException``
    exception if no results are returned and a ``Doctrine\ORM\NonUniqueResultException``
    if *more* than one result is returned. If you use this method, you may
    need to wrap it in a try-catch block and ensure that only one result is
    returned (if you're querying on something that could feasibly return
    more than one result)::
    
        $query = $em->createQuery('SELECT ....')
            ->setMaxResults(1);
        
        try {
            $product = $query->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $product = null;
        }
        // ...

The DQL syntax is incredibly powerful, allowing you to easily join between
entities (the topic of :ref:`relations<book-doctrine-relations>` will be
covered later), group, etc. For more information, see the official Doctrine
`Doctrine Query Language`_ documentation.

.. sidebar:: Setting Parameters

    Take note of the ``setParameter()`` method. When working with Doctrine,
    it's always a good idea to set any external values as "placeholders",
    which was done in the above query:
    
    .. code-block:: text

        ... WHERE p.price > :price ...

    You can then set the value of the ``price`` placeholder by calling the
    ``setParameter()`` method::

        ->setParameter('price', '19.99')

    Using parameters instead of placing values directly in the query string
    is done to prevent SQL injection attacks and should *always* be done.
    If you're using multiple parameters, you can set their values at once
    using the ``setParameters()`` method::

        ->setParameters(array(
            'price' => '19.99',
            'name'  => 'Foo',
        ))

Using Doctrine's Query Builder
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Instead of writing the queries directly, you can alternatively use Doctrine's
``QueryBuilder`` to do the same job using a nice, object-oriented interface.
If you use an IDE, you can also take advantage of auto-completion as you
type the method names. From inside a controller::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();
    
    $products = $query->getResult();

The ``QueryBuilder`` object contains every method necessary to build your
query. By calling the ``getQuery()`` method, the query builder returns a
normal ``Query`` object, which is the same object you built directly in the
previous section.

For more information on Doctrine's Query Builder, consult Doctrine's
`Query Builder`_ documentation.

Custom Repository Classes
~~~~~~~~~~~~~~~~~~~~~~~~~

In the previous sections, you began constructing and using more complex queries
from inside a controller. In order to isolate, test and reuse these queries,
it's a good idea to create a custom repository class for your entity and
add methods with your query logic there.

To do this, add the name of the repository class to your mapping definition.

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        namespace Acme\StoreBundle\Entity;

        use Doctrine\ORM\Mapping as ORM;

        /**
         * @ORM\Entity(repositoryClass="Acme\StoreBundle\Repository\ProductRepository")
         */
        class Product
        {
            //...
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            repositoryClass: Acme\StoreBundle\Repository\ProductRepository
            # ...

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product"
                    repository-class="Acme\StoreBundle\Repository\ProductRepository">
                    <!-- ... -->
            </entity>
        </doctrine-mapping>

Doctrine can generate the repository class for you by running the same command
used earlier to generate the missing getter and setter methods:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Next, add a new method - ``findAllOrderedByName()`` - to the newly generated
repository class. This method will query for all of the ``Product`` entities,
ordered alphabetically.

.. code-block:: php

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    namespace Acme\StoreBundle\Repository;

    use Doctrine\ORM\EntityRepository;

    class ProductRepository extends EntityRepository
    {
        public function findAllOrderedByName()
        {
            return $this->getEntityManager()
                ->createQuery('SELECT p FROM AcmeStoreBundle:Product p ORDER BY p.name ASC')
                ->getResult();
        }
    }

.. tip::

    The entity manager can be accessed via ``$this->getEntityManager()``
    from inside the repository.

You can use this new method just like the default finder methods of the repository::

    $em = $this->getDoctrine()->getEntityManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
                ->findAllOrderedByName();

.. note::

    When using a custom repository class, you still have access to the default
    finder methods such as ``find()`` and ``findAll()``.

.. _`book-doctrine-relations`:

Entity Relationships/Associations
---------------------------------

Suppose that the products in your application all belong to exactly one "category".
In this case, you'll need a ``Category`` object and a way to relate a ``Product``
object to a ``Category`` object. Start by creating the ``Category`` entity.
Since you know that you'll eventually need to persist the class through Doctrine,
you can let Doctrine create the class for you.

.. code-block:: bash

    php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Category" --fields="name:string(255)"

This task generates the ``Category`` entity for you, with an ``id`` field,
a ``name`` field and the associated getter and setter functions.

Relationship Mapping Metadata
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To relate the ``Category`` and ``Product`` entities, start by creating a
``products`` property on the ``Category`` class:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Category.php
        // ...
        use Doctrine\Common\Collections\ArrayCollection;
        
        class Category
        {
            // ...
            
            /**
             * @ORM\OneToMany(targetEntity="Product", mappedBy="category")
             */
            protected $products;
    
            public function __construct()
            {
                $this->products = new ArrayCollection();
            }
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Category.orm.yml
        Acme\StoreBundle\Entity\Category:
            type: entity
            # ...
            oneToMany:
                products:
                    targetEntity: Product
                    mappedBy: category
            # don't forget to init the collection in entity __construct() method


First, since a ``Category`` object will relate to many ``Product`` objects,
a ``products`` array property is added to hold those ``Product`` objects.
Again, this isn't done because Doctrine needs it, but instead because it
makes sense in the application for each ``Category`` to hold an array of
``Product`` objects.

.. note::

    The code in the ``__construct()`` method is important because Doctrine
    requires the ``$products`` property to be an ``ArrayCollection`` object.
    This object looks and acts almost *exactly* like an array, but has some
    added flexibility. If this makes you uncomfortable, don't worry. Just
    imagine that it's an ``array`` and you'll be in good shape.

.. tip::

   The targetEntity value in the decorator used above can reference any entity
   with a valid namespace, not just entities defined in the same class. To 
   relate to an entity defined in a different class or bundle, enter a full
   namespace as the targetEntity.

Next, since each ``Product`` class can relate to exactly one ``Category``
object, you'll want to add a ``$category`` property to the ``Product`` class:

.. configuration-block::

    .. code-block:: php-annotations

        // src/Acme/StoreBundle/Entity/Product.php
        // ...
    
        class Product
        {
            // ...
        
            /**
             * @ORM\ManyToOne(targetEntity="Category", inversedBy="products")
             * @ORM\JoinColumn(name="category_id", referencedColumnName="id")
             */
            protected $category;
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            manyToOne:
                category:
                    targetEntity: Category
                    inversedBy: products
                    joinColumn:
                        name: category_id
                        referencedColumnName: id

Finally, now that you've added a new property to both the ``Category`` and
``Product`` classes, tell Doctrine to generate the missing getter and setter
methods for you:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Ignore the Doctrine metadata for a moment. You now have two classes - ``Category``
and ``Product`` with a natural one-to-many relationship. The ``Category``
class holds an array of ``Product`` objects and the ``Product`` object can
hold one ``Category`` object. In other words - you've built your classes
in a way that makes sense for your needs. The fact that the data needs to
be persisted to a database is always secondary.

Now, look at the metadata above the ``$category`` property on the ``Product``
class. The information here tells doctrine that the related class is ``Category``
and that it should store the ``id`` of the category record on a ``category_id``
field that lives on the ``product`` table. In other words, the related ``Category``
object will be stored on the ``$category`` property, but behind the scenes,
Doctrine will persist this relationship by storing the category's id value
on a ``category_id`` column of the ``product`` table.

.. image:: /images/book/doctrine_image_2.png
   :align: center

The metadata above the ``$products`` property of the ``Category`` object
is less important, and simply tells Doctrine to look at the ``Product.category``
property to figure out how the relationship is mapped.

Before you continue, be sure to tell Doctrine to add the new ``category``
table, and ``product.category_id`` column, and new foreign key:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    This task should only be really used during development. For a more robust
    method of systematically updating your production database, read about
    :doc:`Doctrine migrations</bundles/DoctrineMigrationsBundle/index>`.

Saving Related Entities
~~~~~~~~~~~~~~~~~~~~~~~

Now, let's see the code in action. Imagine you're inside a controller::

    // ...
    use Acme\StoreBundle\Entity\Category;
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    class DefaultController extends Controller
    {
        public function createProductAction()
        {
            $category = new Category();
            $category->setName('Main Products');
            
            $product = new Product();
            $product->setName('Foo');
            $product->setPrice(19.99);
            // relate this product to the category
            $product->setCategory($category);
            
            $em = $this->getDoctrine()->getEntityManager();
            $em->persist($category);
            $em->persist($product);
            $em->flush();
            
            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }

Now, a single row is added to both the ``category`` and ``product`` tables.
The ``product.category_id`` column for the new product is set to whatever
the ``id`` is of the new category. Doctrine manages the persistence of this
relationship for you.

Fetching Related Objects
~~~~~~~~~~~~~~~~~~~~~~~~

When you need to fetch associated objects, your workflow looks just like it
did before. First, fetch a ``$product`` object and then access its related
``Category``::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $categoryName = $product->getCategory()->getName();
        
        // ...
    }

In this example, you first query for a ``Product`` object based on the product's
``id``. This issues a query for *just* the product data and hydrates the
``$product`` object with that data. Later, when you call ``$product->getCategory()->getName()``,
Doctrine silently makes a second query to find the ``Category`` that's related
to this ``Product``. It prepares the ``$category`` object and returns it to
you.

.. image:: /images/book/doctrine_image_3.png
   :align: center

What's important is the fact that you have easy access to the product's related
category, but the category data isn't actually retrieved until you ask for
the category (i.e. it's "lazily loaded").

You can also query in the other direction::

    public function showProductAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Category')
            ->find($id);

        $products = $category->getProducts();
    
        // ...
    }

In this case, the same things occurs: you first query out for a single ``Category``
object, and then Doctrine makes a second query to retrieve the related ``Product``
objects, but only once/if you ask for them (i.e. when you call ``->getProducts()``).
The ``$products`` variable is an array of all ``Product`` objects that relate
to the given ``Category`` object via their ``category_id`` value.

.. sidebar:: Relationships and Proxy Classes

    This "lazy loading" is possible because, when necessary, Doctrine returns
    a "proxy" object in place of the true object. Look again at the above
    example::
    
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // prints "Proxies\AcmeStoreBundleEntityCategoryProxy"
        echo get_class($category);

    This proxy object extends the true ``Category`` object, and looks and
    acts exactly like it. The difference is that, by using a proxy object,
    Doctrine can delay querying for the real ``Category`` data until you
    actually need that data (e.g. until you call ``$category->getName()``).

    The proxy classes are generated by Doctrine and stored in the cache directory.
    And though you'll probably never even notice that your ``$category``
    object is actually a proxy object, it's important to keep in mind.

    In the next section, when you retrieve the product and category data
    all at once (via a *join*), Doctrine will return the *true* ``Category``
    object, since nothing needs to be lazily loaded.

Joining to Related Records
~~~~~~~~~~~~~~~~~~~~~~~~~~

In the above examples, two queries were made - one for the original object
(e.g. a ``Category``) and one for the related object(s) (e.g. the ``Product``
objects).

.. tip::

    Remember that you can see all of the queries made during a request via
    the web debug toolbar.

Of course, if you know up front that you'll need to access both objects, you
can avoid the second query by issuing a join in the original query. Add the
following method to the ``ProductRepository`` class::

    // src/Acme/StoreBundle/Repository/ProductRepository.php
    
    public function findOneByIdJoinedToCategory($id)
    {
        $query = $this->getEntityManager()
            ->createQuery('
                SELECT p, c FROM AcmeStoreBundle:Product p
                JOIN p.category c
                WHERE p.id = :id'
            )->setParameter('id', $id);
        
        try {
            return $query->getSingleResult();
        } catch (\Doctrine\ORM\NoResultException $e) {
            return null;
        }
    }

Now, you can use this method in your controller to query for a ``Product``
object and its related ``Category`` with just one query::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();
    
        // ...
    }    

More Information on Associations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This section has been an introduction to one common type of entity relationship,
the one-to-many relationship. For more advanced details and examples of how
to use other types of relations (e.g. ``one-to-one``, ``many-to-many``), see
Doctrine's `Association Mapping Documentation`_.

.. note::

    If you're using annotations, you'll need to prepend all annotations with
    ``ORM\`` (e.g. ``ORM\OneToMany``), which is not reflected in Doctrine's
    documentation. You'll also need to include the ``use Doctrine\ORM\Mapping as ORM;``
    statement, which *imports* the ``ORM`` annotations prefix.

Configuration
-------------

Doctrine is highly configurable, though you probably won't ever need to worry
about most of its options. To find out more about configuring Doctrine, see
the Doctrine section of the :doc:`reference manual</reference/configuration/doctrine>`.

Lifecycle Callbacks
-------------------

Sometimes, you need to perform an action right before or after an entity
is inserted, updated, or deleted. These types of actions are known as "lifecycle"
callbacks, as they're callback methods that you need to execute during different
stages of the lifecycle of an entity (e.g. the entity is inserted, updated,
deleted, etc).

If you're using annotations for your metadata, start by enabling the lifecycle
callbacks. This is not necessary if you're using YAML or XML for your mapping:

.. code-block:: php-annotations

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Product
    {
        // ...
    }

Now, you can tell Doctrine to execute a method on any of the available lifecycle
events. For example, suppose you want to set a ``created`` date column to
the current date, only when the entity is first persisted (i.e. inserted):

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * @ORM\PrePersist
         */
        public function setCreatedValue()
        {
            $this->created = new \DateTime();
        }

    .. code-block:: yaml

        # src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.yml
        Acme\StoreBundle\Entity\Product:
            type: entity
            # ...
            lifecycleCallbacks:
                prePersist: [ setCreatedValue ]

    .. code-block:: xml

        <!-- src/Acme/StoreBundle/Resources/config/doctrine/Product.orm.xml -->
        <!-- ... -->
        <doctrine-mapping>

            <entity name="Acme\StoreBundle\Entity\Product">
                    <!-- ... -->
                    <lifecycle-callbacks>
                        <lifecycle-callback type="prePersist" method="setCreatedValue" />
                    </lifecycle-callbacks>
            </entity>
        </doctrine-mapping>

.. note::

    The above example assumes that you've created and mapped a ``created``
    property (not shown here).

Now, right before the entity is first persisted, Doctrine will automatically
call this method and the ``created`` field will be set to the current date.

This can be repeated for any of the other lifecycle events, which include:

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

For more information on what these lifecycle events mean and lifecycle callbacks
in general, see Doctrine's `Lifecycle Events documentation`_

.. sidebar:: Lifecycle Callbacks and Event Listeners

    Notice that the ``setCreatedValue()`` method receives no arguments. This
    is always the case for lifecycle callbacks and is intentional: lifecycle
    callbacks should be simple methods that are concerned with internally
    transforming data in the entity (e.g. setting a created/updated field,
    generating a slug value).
    
    If you need to do some heavier lifting - like perform logging or send
    an email - you should register an external class as an event listener
    or subscriber and give it access to whatever resources you need. For
    more information, see :doc:`/cookbook/doctrine/event_listeners_subscribers`.

Doctrine Extensions: Timestampable, Sluggable, etc.
---------------------------------------------------

Doctrine is quite flexible, and a number of third-party extensions are available
that allow you to easily perform repeated and common tasks on your entities.
These include thing such as *Sluggable*, *Timestampable*, *Loggable*, *Translatable*,
and *Tree*.

For more information on how to find and use these extensions, see the cookbook
article about :doc:`using common Doctrine extensions</cookbook/doctrine/common_extensions>`.

.. _book-doctrine-field-types:

Doctrine Field Types Reference
------------------------------

Doctrine comes with a large number of field types available. Each of these
maps a PHP data type to a specific column type in whatever database you're
using. The following types are supported in Doctrine:

* **Strings**

  * ``string`` (used for shorter strings)
  * ``text`` (used for larger strings)

* **Numbers**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **Dates and Times** (use a `DateTime`_ object for these fields in PHP)

  * ``date``
  * ``time``
  * ``datetime``

* **Other Types**

  * ``boolean``
  * ``object`` (serialized and stored in a ``CLOB`` field)
  * ``array`` (serialized and stored in a ``CLOB`` field)

For more information, see Doctrine's `Mapping Types documentation`_.

Field Options
~~~~~~~~~~~~~

Each field can have a set of options applied to it. The available options
include ``type`` (defaults to ``string``), ``name``, ``length``, ``unique``
and ``nullable``. Take a few examples:

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * A string field with length 255 that cannot be null
         * (reflecting the default values for the "type", "length" and *nullable* options)
         * 
         * @ORM\Column()
         */
        protected $name;
    
        /**
         * A string field of length 150 that persists to an "email_address" column
         * and has a unique index.
         *
         * @ORM\Column(name="email_address", unique=true, length=150)
         */
        protected $email;

    .. code-block:: yaml

        fields:
            # A string field length 255 that cannot be null
            # (reflecting the default values for the "length" and *nullable* options)
            # type attribute is necessary in yaml definitions
            name:
                type: string

            # A string field of length 150 that persists to an "email_address" column
            # and has a unique index.
            email:
                type: string
                column: email_address
                length: 150
                unique: true

.. note::

    There are a few more options not listed here. For more details, see
    Doctrine's `Property Mapping documentation`_

.. index::
   single: Doctrine; ORM Console Commands
   single: CLI; Doctrine ORM

Console Commands
----------------

The Doctrine2 ORM integration offers several console commands under the
``doctrine`` namespace. To view the command list you can run the console
without any arguments:

.. code-block:: bash

    php app/console

A list of available command will print out, many of which start with the
``doctrine:`` prefix. You can find out more information about any of these
commands (or any Symfony command) by running the ``help`` command. For example,
to get details about the ``doctrine:database:create`` task, run:

.. code-block:: bash

    php app/console help doctrine:database:create

Some notable or interesting tasks include:

* ``doctrine:ensure-production-settings`` - checks to see if the current
  environment is configured efficiently for production. This should always
  be run in the ``prod`` environment:
  
  .. code-block:: bash
  
    php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - allows Doctrine to introspect an existing
  database and create mapping information. For more information, see
  :doc:`/cookbook/doctrine/reverse_engineering`.

* ``doctrine:mapping:info`` - tells you all of the entities that Doctrine
  is aware of and whether or not there are any basic errors with the mapping.

* ``doctrine:query:dql`` and ``doctrine:query:sql`` - allow you to execute
  DQL or SQL queries directly from the command line.

.. note::

   To be able to load data fixtures to your database, you will need to have
   the ``DoctrineFixturesBundle`` bundle installed. To learn how to do it,
   read the ":doc:`/bundles/DoctrineFixturesBundle/index`" entry of the
   documentation.

Summary
-------

With Doctrine, you can focus on your objects and how they're useful in your
application and worry about database persistence second. This is because
Doctrine allows you to use any PHP object to hold your data and relies on
mapping metadata information to map an object's data to a particular database
table.

And even though Doctrine revolves around a simple concept, it's incredibly
powerful, allowing you to create complex queries and subscribe to events
that allow you to take different actions as objects go through their persistence
lifecycle.

For more information about Doctrine, see the *Doctrine* section of the
:doc:`cookbook</cookbook/index>`, which includes the following articles:

* :doc:`/bundles/DoctrineFixturesBundle/index`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Basit Eşleme Belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html
.. _`Query Builder`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/query-builder.html
.. _`Doctrine Query Language`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/dql-doctrine-query-language.html
.. _`Association Mapping Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Mapping Types Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#doctrine-mapping-types
.. _`Property Mapping documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/events.html#lifecycle-events
.. _`Ayrılmış SQL anahtar kelimeleri belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#quoting-reserved-words
