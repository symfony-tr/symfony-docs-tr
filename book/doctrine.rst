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

Getter'ları ve Setter'ları Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Doctrine şimdi ``Product`` nesnesini nasıl veritabanına yazacağını bilmesine
rağmen bu sınıf hala kullanışlı bir sınıf değil. ``Product`` sınıfı sadece
düz bir PHP sınıfı olduğundan dolayı sınıfın değişkenlerine erişebilmek
için (sınıfın değişkenleri(properties) protected tipinde olduğu için) bazı
metod fonksiyonları (örn: ``getName()``, ``setName()``) yaratmanız gereklidir.
Çok şükür ki Doctrine şunu çalıştırdığınızda bunu sizin için yapar:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product

Bu komut yaratılan ``Product`` sınıfı için tüm getter'ları ve setter'ları
yaratılmasını sağlar.
Bu güvenli bir komuttur. Tekrar tekrar çalıştırabilirsiniz.Çünki komut sadece
yaratılmayan değişkenler (properties) için getter ve setter metodları yaratır.

.. sidebar:: Daha Fazlası ``doctrine:generate:entities``

    ``doctrine:generate:entities`` komutu ile şunları yapabilirsiniz:

        * getter  ve setter metodları yapabilirsiniz,

        * ``@ORM\Entity(repositoryClass="...")`` belirteci ile 
          konfigüre edilebilien ambar (repository) sınıfları yaratabilirsiniz,

        * 1:n ve n:m ilişkilere uygun yapıcılar (constructor) yaratabilirsiniz.

    
    ``doctrine:generate:entities`` komutu orijinal ``Product.php`` dosyasını
    ``Product.php~`` olarak bir yedeğini alır. Bazı durumlarda bu dosyanın 
    varlığı "Cannot redeclare class" hatası verebilir.Bu dosyayı güvenle silebilirsiniz.

    
    Bu komuta ihiyacınız *olmadığını* hatırlatalım. Doctrine kod yaratım 
    araçlarına dikkat etmez. Normal PHP sınıfları gibi sadece sınıfınızdaki
    protected tipinde  değişkenlerin (properties) olması ve bu değişkenlere
    erişebilmek için ilgili getter ve setter metod fonksiyonlarının olması
    yeterlidir. Bu konu Doctrine içerisinde önemli olmasından dolayı bu komut
    bunları sizin için yaratır.

Ayrıca var olan tüm bundle'lar içerisinde ya da bundle içerisindeki 
namespace bilgisi içerisinde kalan tüm entity'lerin (Örn. Doctrine eşleme
bilgisi içeren herhangi bir PHP sınıfı) getter ve setter'larının yaratımını
yapabilirsiniz: 

.. code-block:: bash

    php app/console doctrine:generate:entities AcmeStoreBundle
    php app/console doctrine:generate:entities Acme

.. note::

    Doctrine sınıf değişkenlerinizin ``protected`` ya da ``private`` tipte 
    olması ile ya da değişkenin bir getter ya da setter fonksiyonuna sahip
    olması ile ilgilenmez. Getter ve setter metodları siz bu PHP sınıfı ile
    etkileşime geçmek istediğinizde bu etkileşimi sağlamak içindir.

Veritabanı Tabloları/Şemaları Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Artık Doctrine'nin de nasıl veritabanına yazacağını açıkça bildiği kullanışlı
bir ``Product`` sınıfınız var. Elbette veritabanınız içerisinde ilgili 
``product`` tablonuz yok. Çok şükür ki Doctrine uygulamanız içinde kullandığınız
tüm entity'ler için gerekli olan tabloları otomatik olarak yaratır.
Bunu şu komutu çalıştırarak yapabilirsiniz:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. tip::

    Gerçekte bu komut inanılmaz güçlüdür. Bu araç veritabanınızı nasıl olması
    gerektiğinle (entity'lerinizdeki eşleme bilgisine göre) ve gerçekte 
    nasıl olduğunla karşılaştırır ve olması gereken değişikliklere göre
    gerekli SQL cümlelerini çalıştrırarak veritabanınızı günceller.
    Başka bir ifade ile eğer ``Product`` sınıfına yeni bir değişken 
    (property) eklerseniz ve bu komutu yeniden çalıştırırsanız, komut
	"alter table" ifadesini çalıştıracak ve ``product`` tablosundaki
	gerekli alanı yaratacaktır.
    
    An even better way to take advantage of this functionality is via
    :doc:`migrations</bundles/DoctrineMigrationsBundle/index>`, which allow you to
    generate these SQL statements and store them in migration classes that
    can be run systematically on your production server in order to track
    and migrate your database schema safely and reliably.


Veritabanınız şimdi belirmiş olduğunuz eşleme bilgisine uygun sütunlara sahip,
tam fonksiyonel bir ``product`` tablosuna sahip.

Nesneleri Veritabanına Yazmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Şimdi elinizde ilgili ``product`` tablosu ile eşleştirilmiş (map) ``Product`` entitysi 
var ve veritabanına yazmaya hazırsınız. Controller içerisinde bu oldukça basittir.
Bundle'ınız içerisindeki ``DefaultController`` içerisine şu satırı ekleyin: 

.. code-block:: php
    :linenos:

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Entity\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...
    
    public function createAction()
    {
        $product = new Product();
        $product->setName('Örnek Ürün Adı');
        $product->setPrice('19.99');
        $product->setDescription('Bir Açıklama');

        $em = $this->getDoctrine()->getEntityManager();
        $em->persist($product);
        $em->flush();

        return new Response('Yaratılan Ürün id: '.$product->getId());
    }

.. note::

    Eğer bunu bir örnek ile birlikte takip ediyorsanız bunun çalışması 
    için kendinize bir route (yönlendirme) ayarı yapmanız gereklidir.

Şimdi bu örnekten devam edelim:

* **8-11 satırlar** Bu kısımda diğer normal PHP nesneleri ile çalıştığınız gibi 
  ``$product`` ile temsil edilen bir değişkende işlemler yaptınız;

* **satır 13** Bu satır Doctrine'in nesneleri okumak ya da 
  veritabanına yazmak gibi işlemlerinde kullandığı *entity manager* nesnesini alır;

* **satır 14** ``persist()`` metodu Doctrine bu nesnenin yönetileceğini söyler.
  Şu anda gerçek olarak veritabanına herhangi bir yazım işlemi olmadı (şimdilik).

* **satır 15** ``flush()`` metodu çağırıldığında Doctrone veritabanına yazılmak için
  yönetilecek olan tüm nesneleri alır ve veritabanına yazar. Bu örnekte  ``$product``
  nesnesi henüz yazılmadı bu yüzden entity yöneticisi (entity manager) ``INSERT``
  ifadesini kullanarak ``product`` tablosunda bir satır yarattı.

.. note::

  Aslında Doctrine tüm yönetilen entity'lerinize dikkat etmesine rağmen
  ne zaman ``flush()`` metodunu çağırdığınızda yapılacak tüm değişiklikler
  hesaplanır ve mümkün olan en etkin sorgu/sorgular şeklinde çalıştırılır.
  Örneğin eğer toplamda 100 ürün nesnesi yazacaksanız ve en altta ``flush()`` 
  metodunu çağırırsanız Doctrine *bir adet* hazırlanmış ifade yaratır ve
  her birisi için bu ifadeyi tekrar tekrar kullanır. Bu tip kullanıma 
  *Unit of Work* denir. Bu tür bir yapı kullanılır çünki bu hızlı ve verimlidir.


Nesneleri yaratırken ya da güncellerken akış sistemi hep aynıdır. Sonraki
bölümde Doctrine'nin nasıl akıllı bir şekilde veri tabanıda var olan bir 
kayıt için ``UPDATE`` ifadesini kullandığını göreceksiniz.

.. tip::

    Doctrine size projeniz içerisinde programsal olarak test yapabilmeniz 
    için bir kütüphane de sağlar. Daha fazla bilgi için 
    :doc:`/bundles/DoctrineFixturesBundle/index`
    belgesine bakın.

Veritabanından Nesneleri Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Veritabanından bir nesneyi almak çok kolaydır. Örneğin varsayalım 
``Product`` nesnesinin ``id`` değeri üzerinden bir yönlendirme (route)
yapılandırdınız::

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

Sorgu bir nesnenin bir parçası olduğunda her zaman onu bir "ambar" gibi
kullanabilirsiniz. Repository'i (ambar), işi sadece entity'leri belirli bir sınıf
için almak olan bir PHP sınıfı olarak düşünebilirsiniz. Bir entity sınıfını 
kullanarak bu repositry sınıfına şu şekilde ulaşabilirsiniz::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    ``AcmeStoreBundle:Product`` ifadesi Sınıfın tam adını kullanmak yerine 
    (Örn. ``Acme\StoreBundle\Entity\Product``) Doctrine içerisinde herhangi bir
    yerde bu sınıfı kullanmak için onu kısaca tanımlayan ifadedir.
    Entity sınıfı bundle'ınız içerisindeki ``Entity`` isim uzayında olduğu
    sürece bu çalışacaktır.

Bir kez bir repository'niz olursa aşağıda sıralanan tüm faydalı metodlara
ulaşabilirsiniz::

    // primary key'e göre sql sorgusu (genellikle "id")
    $product = $repository->find($id);

    // sütün değerine göre bulmak için dinamik metodlar
    $product = $repository->findOneById($id);
    $product = $repository->findOneByName('foo');

    // *Tüm* ürünleri getirir
    $products = $repository->findAll();

    // Rastgele seçilen sütun değerine uyan tüm verileri gurup halinde
    // getirir.
    $products = $repository->findByPrice(19.99);

.. note::

    Elbette karmaşık sorgular yazmak isterseniz :ref:`book-doctrine-queries`
    kısmına da bakabilirsiniz.

Ayrıca ``findBy`` and ``findOneBy`` metodlarını birden fazla  şartla uyan nesneleri 
almak içinde kullanabilirsiniz::

    // Fiyat ve isime uyan ürünleri getir.
    $product = $repository->findOneBy(array('name' => 'foo', 'price' => 19.99));

    // qfiyata göre sıralı bir şekilde tüm ürünleri getir.
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

   Herhangi bir sayfayı ekrana bastığınızda ne kadar sorgunun çalıştığını
   görme isterseniz web debug araç çubuğunun sağ alt köşesine bakın.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    Eğer ikon'a tıklarsanız profiller açılacak ve size ne kadar sorgunun
    yapıldığını açıkça gösterecektir.

Bir Nesneyi Güncellemek
~~~~~~~~~~~~~~~~~~~~~~~~
Bir kez Doctrinden bir nesneyi aldınız mı güncellemek çok kolaydır.
Varsayalım ürün id'sine göre controllerda update action'a giden bir route'nız
var::

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

Güncellenen nesne şu üç adımdan oluşur:

1. Doctrine üzerinden nesneyi al;
2. nesneyi değiştir;
3. entity manager üzerinden ``flush()``metodunu çağır.

``$em->persist($product)``  metodunun çağırılmasının gereksiz olduğunu hatırlatalım.
Bu  metodun yeniden çağırımı Doctrine'e sadece ``$product`` nesnesini yönet
ya da izle demektir.Bu durumda ``$product`` nesnesini Doctrine üzerinden aldığınız
için bu zaten yönetilebilir duruma gelmiştir.

Bir Nesneyi Silmek
~~~~~~~~~~~~~~~~~~

Bir nesneyi silmekde çok benzer ancak entity manager üzerinden bu sefer
``remove()`` metodunu çağırılması gerekir::


    $em->remove($product);
    $em->flush();


``remove()`` metodu Doctrine istenilen entity'in veritabanından silinmesini
söyler. Gerçekte bu ``DELETE`` sorgusu olmasına karşın ``flush()`` metodu
çağırılmadan çalışmaz. 

.. _`book-doctrine-queries`:

Nesneler için Sorgulama
-----------------------
Bir repository(ambar) nesnesinin herhangi bir işlem yapmadan basit
sorguları nasıl çalıştırdığını zaten gördünüz::

    $repository->find($id);
    
    $repository->findOneByName('Foo');


Elbette Doctrine ayrıca daha karmaşık sorgular için Doctrine Sorgu Dili
adındaki (DQL) bir araçla bu sorguları yazmanıza olanak sağlar.DQL , SQL
diline benzer olarak sadece SQL'deki gibi tablodaki satırlar için sorgular
yazmaktan ziyade (Örn: ``product``) bir entity snıfı ya da birden fazla
entity sınıfı için sorgu yazmanız gereklidir. (Örn ``Product``)

Doctrine üzerinde sorgu yazarken iki seçeneğiniz bulunmaktadır. Birincisi
saf Doctrine sorguları yazmak, diğeri ise Doctrine Sorgu Üreteci'ni (Query Builder)
kullanmaktır.

Nesneleri DQL ile Sorgulamak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Farzedin ki ürünlerinizi sorgulamak istiyorsunuz ancak sadece fiyatı ``19.99``
dan büyük olanları sorgulamak istiyorsunuz ve bunlar en ucuzdan en pahalıya 
doğru sıralanacak şekilde geri dönecek.  Bir controller içinden şunu yapabilirsiniz::

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();


Eğer SQL 'de rahat ediyorsanız DQL üzerinde daha rahat hissedeceksiniz. Buradaki
en büyük fark veri tabanı üzerindeki sorgunuzu satırlar olarak düşünmek yerine
"nesneler" olarak düşünmeniz gerekliliğidir. Bu yüzden 
select *from*  ``AcmeStoreBundle:Product``
ifadesine ``p`` aliası (takma adı) verilmiştir. 

``getResult()`` metodu sonuçları bir dize (array) halinde döndürür. Eğer sadece
bir nesne dönüşü için sorguladıysanız bu durumda ``getSingleResult()`` metodunu
bu metodun yerine kullanabilirsiniz::

    $product = $query->getSingleResult();

.. caution::

    ``getSingleResult()`` metodu herhangibir sonuç dönmemesi halinde bir 
    ``Doctrine\ORM\NoResultException`` istisnası ve eğer birden fazla sonuç döndüyse
    ``Doctrine\ORM\NonUniqueResultException`` istisnası atar. Eğer bu metodu
    kullanırsanız mutlaka bu işlemi try catch içerisine alarak sadece bir sonucun
    dönmesi durumunu kontrol etmeniz gerekir (eğer sorguladığınız herhangi birşey
    birden fazla sonuç döndürme olasılığı varsa)::
    
        $query = $em->createQuery('SELECT ....')
            ->setMaxResults(1);
        
        try {
            $product = $query->getSingleResult();
        } catch (\Doctrine\Orm\NoResultException $e) {
            $product = null;
        }
        // ...


DQL yazımı (syntax) entity 'ler arasında join, group işlemleri için 
(:ref:`ilişkiler<book-doctrine-relations>` konusu daha sonra işlenecektir)
inanılmaz güçlüdür. Daha fazla bilgi için resmi `Doctrine Sorgu Dili`_ 
belgesine bakın.

.. sidebar:: Parametreleri Ayarlamak

    ``setParameter()`` metoduna dikkat edin. Doctrine ile çalışırken
    herhangi bir dışsal değeri bir "yertutucu"  içerisine atıp, query
    içerisinde kullanmak her zaman çok iyi bir yöntemdir:
    
    .. code-block:: text

        ... WHERE p.price > :price ...

    ``price`` yer tutucusunun değerini ``setParameter()`` metodunu 
    çağırarak düzenleyebilirsiniz:

        ->setParameter('price', '19.99')

    Sorgu içerisinde değerleri direkt olarak vermek yerine yertutucularını 
    kullanmak *daima* SQL injection ataklarına karşı koruma sağlar.
    Eğer çoklu parametreler kullanıyorsanız, öncelikle onların değerlerini
    ``setParameters()`` metodu ile düzenlemeniz gerekir::

        ->setParameters(array(
            'price' => '19.99',
            'name'  => 'Foo',
        ))

Doctrine'nin Sorgu Üretecini Kullanmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sorguları direkt yazmak yerine alternatif olarak Doctrine'nin ``QueryBuilder``
(Sorgu Üreteci) 'ni kullanarak bu işi nesne yönelimli bir ara birim ile 
daha güzel halledebilirsiniz. Eğer bir IDE kullanıyorsanız aynı zamanda
metodları otomatik tamamlama özelliğinide kullanabilirsiniz. Bir Controller
içerisinden şu şekilde kullanabilirsiniz::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

    $query = $repository->createQueryBuilder('p')
        ->where('p.price > :price')
        ->setParameter('price', '19.99')
        ->orderBy('p.price', 'ASC')
        ->getQuery();
    
    $products = $query->getResult();


``QueryBuilder``  nesnesi sorgunuzu üretebilmek için tüm metodları içerir.
``getQuery()`` metodunun çağırılması ile sorgu üreteci önceki bölümde gösterildiği gibi
normal bir ``Query`` nesnesi çevirir.

Doctrine'nin Sorgu Üreteci (Query Builder) hakkında daha fazla bilgi almak
için `Sorgu Üreteci`_  dökümanına başvurun.

Özel Ambar(Repository) Sınıfları
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Önceki bölümlerde controller içerisinden karmaşık sorguları üretmeye başlamıştınız.
Ancak bu sorguları test etmek ve yeniden kullanmak için izole ederek bunları entity'nizin 
kullanabileceği özel bir repository (ambar) sınıfı içerisine almak ve 
sorguları mantıksal olarak metodlara atamak daha iyi bir fikirdir.

Bunu yapmak için repository(ambar) sınıfınızın ismini eşleştirme (mapping) tanımlamanız
içerisine eklemeniz gereklidir.

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


Doctrine daha önceleri gördüğümüz, getter ve setter metodlarını yaratmak için
kullandığımız komut ile bu repository sınıflarını da yaratabilir:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Sonra bu repository sınıfının içerisine ``findAllOrderedByName()`` adına
yeni bir metod ekleyin. Bu metod tüm ``Product`` entity'lerini sorgulayacak ve
alfabetik olarak sıralayacaktır.

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

    Entity manager'a repository içerisinden ``$this->getEntityManager()``
    ile erişilebilir.

Repository'niz içerisinde bu yeni metodu varsayılan bulma metodu yerine 
basitçe şu şekilde kullanabilirsiniz::

    $em = $this->getDoctrine()->getEntityManager();
    $products = $em->getRepository('AcmeStoreBundle:Product')
                ->findAllOrderedByName();

.. note::

    Özel bir repository sınıfı kullandığınızda diğer ``find()`` ve ``findAll()``
    metodlarına da kolaylıkla erişebilirsiniz.

.. _`book-doctrine-relations`:

Entity İlişkileri/Ortaklıkları
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
.. _`Sorgu Üreteci`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/query-builder.html
.. _`Doctrine Sorgu Dili`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/dql-doctrine-query-language.html
.. _`Association Mapping Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Mapping Types Documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#doctrine-mapping-types
.. _`Property Mapping documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Events documentation`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/events.html#lifecycle-events
.. _`Ayrılmış SQL anahtar kelimeleri belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#quoting-reserved-words
