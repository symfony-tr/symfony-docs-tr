.. index::
   single: Doctrine

Veritabanları ve Doctrine
=========================

Şunu kabul edelim. Herhangi bir uygulamanın en zorlu işleri, veritabanından
bilgileri okumak ya da buraya bilgileri yazmaktır. Çok şükür, Symfony 
`Doctrine`_ adındaki bu işleri kolaylıkla yapabileceğiniz araçları içeren bir
kütüphane ile birlikte entegre gelir. Bu kısımda Doctrine'ın arkasındaki
temel felsefeyi öğrenecek ve veritabanları ile çalışmanın ne kadar kolay
olacağını göreceksiniz. 

.. note::

    Doctrine, isteğe bağlı olarak kullanabilmeniz için Symfony2'den tamamen 
    ayrıştırılmıştır. Bu kısım tamamen Doctrine ORM ile nesnelerinizi ilişkisel
    veri tabanında  (*MySQL*, *PostgreSQL* ya da  *Microsoft SQL*) nasıl 
    kullanacağınızı içerir.
    Eğer saf veritabanı sorgu cümlecikleri kullanmak isterseniz bu da basittir 
    ve ":doc:`/cookbook/doctrine/dbal`" tarif kitabı girdisinde açıklanmıştır.
    Ayrıca verilerinizi Doctrine ODM kullanarak `MongoDB`_ üzerinde de
    işleyebilirsiniz. Bu konudaki daha fazla bilgi için 
    ":doc:`/bundles/DoctrineMongoDBBundle/index`" belgesini okuyabilirsiniz.

Basit Bir Örnek: Bir Ürün (Product)
-----------------------------------

Doctrine'nin nasıl çalıştığını anlamanın en iyi yolu onu uygulamada görmektir.
Bu kısımda veritabanını konfigüre edecek, ``Product`` nesnesi yaratacak,
bunu veri tabanına yazacak ve veritabanından geri çağıracaksınız.

.. sidebar:: Kodu örnek üzerinde takip etmek

    Eğer bu kısmı bir örnekle birlikte takip etmek istiyorsanız,
    şu komut satırı komutu ile ``AcmeStoreBundle`` bundle'ını yaratın:
    
    .. code-block:: bash
    
        php app/console generate:bundle --namespace=Acme/StoreBundle

Veritabanını Konfigüre Etmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Gerçekten başlamadan önce veritabanı bağlantı bilgisini konfigüre etmek
gerekli. Kural olarak bu bilgiler genellikle ``app/config/parameters.ini``
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

   ``parameters.ini`` dosyası ile konfigüre etmek işi sadece genel bir uygulamadır.
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


Bundan sonra Doctine veritabanınızı tanıyor ve şu şekilde de bir
veritabanı yaratabilir:

.. code-block:: bash

    php app/console doctrine:database:create

Bir Entity (Varlık) Sınıfı Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Varsayalım ürünleri listeleyen bir uygulama geliştiriyorsunuz. Bunu 
Doctrine ya da veritabanları olmadan da tasarlasanız bile zaten tüm ürünleri
temsil eden bir ``Product`` nesnesine ihtiyacınız olacaktır.
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
nasıl *eşleşeceğini* (map) düzenleyecek şekilde konfigüre edilmesi gerekir.
Bu metadata YAML, XML gibi formatlarda olacağı gibi ``Product`` sınıfı
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

    
    Tablo ismi isteğe bağlıdır ve eğer belirtilmeze tablo adı otomatik olarak 
    entity sınıfının ismi olarak belirlenir.

Doctrine geniş bir yelpazede çeşitli alan tip tanımlamalarını her birisinin
kendi özel kullanımlarıyla tanıyabilir. Var olan alan tipleri için 
:ref:`book-doctrine-field-types` kısmına bakın.

.. seealso::
    
    Aynı zamanda  eşleme bilgisi hakkında tüm herşey için Doctrine'nin 
    `Basit Eşleme Belgesi`_  belgesine de bakabilirsiniz.
    Eğer belirteç (annotation) kullanıyorsanız bu belirteçleri önlerine
    ``ORM\`` ifadesini eklemeniz gereklidir. (Örn. : ``ORM\Column(..)``)
    Bu Doctrine'nin kendi belgelerinde gösterilmemektedir. Ayrıca 
    ``use Doctrine\ORM\Mapping as ORM;`` ifadesini kullanarak ``ORM``
    belirteci ön ekini *kullanabilir* hale gelirsiniz.
    
.. caution::

   Sınıf isimleri ve sınıfın değişken isimleri, korunan SQL anahtar
   kelimeleri eşleştirilmeyeceğine dikkat edin (örn : ``group`` ya da ``user``).
   Örneğin entity sınıfınızın ismi ``Group`` ise varsayılan olarak
   tablo isminiz ``group`` olacaktır ve bazı motorlarda bu durum SQL
   hatasına yol açar. Bu konudaki geniş bilgi için
   `Ayrılmış SQL anahtar kelimeleri belgesi`_ 'ne bakarak hangi isimleri
   kullanmamanız gerektiğini görebilirsiniz. Alternatif olarak
   eğer veritabanı şeması seçmekte özgür iseniz basitçe bu isimler için
   farklı tablo ya da sütün adı kullanabilirsiniz. Doctrine'nin   
   `Eşleştirme Tipleri Belgesi`_ ve `Kalıcı sınıflar`_ belgesine bakın.

.. note::

    Belirteçleri kullanan başka bir kütüphane ya da program kullandığınızda
    (örn: Doxygen) ``@IgnoreAnnotation`` belirtecini kullanarak Symfony' nin
    hangi belirteçleri görmezden geleceğini ayarlamanız gereklidir.
    
    Örneğin, ``@fn`` belirtecinin bir hataya sebep olmaması için şu ifadeyi 
    kullanmanız gerekmektedir::

        /**
         * @IgnoreAnnotation("fn")
         */
        class Product

Getter'ları ve Setter'ları Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Doctrine ``Product`` nesnesini veri tabanına nasıl yazacağını bilmesine
rağmen bu sınıf hala kullanışlı bir sınıf değil. ``Product`` sınıfı sadece
düz bir PHP sınıfı olduğundan dolayı sınıfın değişkenlerine erişebilmek
için (sınıfın değişkenleri(properties) protected tipinde olduğu için) bazı
metod fonksiyonları (örn: ``getName()``, ``setName()``) yaratmanız gereklidir.
Çok şükür ki Doctrine şunu çalıştırdığınızda bunların hepsini sizin için yapar:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme/StoreBundle/Entity/Product

Bu komut, yaratılan ``Product`` sınıfı için tüm getter'ları ve setter'ları
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
    varlığı "Cannot redeclare class" hatası verebilir.
    Bu dosyayı güvenle silebilirsiniz.

    Bu komuta ihiyacınız *olmadığını* hatırlatalım. Doctrine kod yaratım 
    araçlarına dikkat etmez. Normal PHP sınıfları gibi sadece sınıfınızdaki
    protected tipinde sınıf değişkenlerinin (properties) olması ve bu değişkenlere
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

Artık Doctrine'nin nasıl veritabanına yazacağını açıkça bildiği kullanışlı
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
    
    Bu özelliğin daha büyük bir avantajı ise migration sınıfları ile 
    sistematik bir şekilde ürün sunucunuzdan migrate edeceğiniz 
    veritabanı şemasına göre bu SQL cümleciklerini yaratan
    :doc:`migration</bundles/DoctrineMigrationsBundle/index>` (göçerme)
    dir.

Veritabanınız şimdi belirmiş olduğunuz eşleme bilgisine uygun sütunlara sahip,
fonksiyonel bir ``product`` tablosuna sahip.

Nesneleri Veritabanına Yazmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şimdi elinizde ilgili ``product`` tablosu ile eşleştirilmiş (map) ``Product`` entity
nesnesi var ve veritabanına yazmaya hazırsınız. Controller içerisinde bu oldukça basittir.
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

* **satır 14** ``persist()`` metodu Doctrine bu nesnenin nasıl yönetileceğini söyler.
  Şu anda gerçek olarak veritabanına herhangi bir yazım işlemi olmadı (şimdilik).

* **satır 15** ``flush()`` metodu çağırıldığında Doctrine veritabanına yazılmak için
  yönetilecek olan tüm nesneleri alır ve veritabanına yazar. Bu örnekte  ``$product``
  nesnesi önceden veritabanına yazılmadı bu yüzden entity yöneticisi (entity manager) ``INSERT``
  ifadesini kullanarak ``product`` tablosunda bir satır yarattı.

.. note::

  Aslında Doctrine tüm yönetilen entity'lerinize dikkat etmesine rağmen
  ne zaman ``flush()`` metodunu çağırdığınızda yapılacak tüm değişiklikler
  hesaplanır ve mümkün olan en etkin sorgu/sorgular şeklinde çalıştırılır.
  Örneğin eğer toplamda 100 ürün nesnesi yazacaksanız ve en altta ``flush()`` 
  metodunu çağırırsanız Doctrine *bir adet* hazırlanmış SQL ifadesi yaratır ve
  her birisi için bu ifadeyi tekrar tekrar kullanır. Bu tip kullanıma 
  *Unit of Work* denir. Bu tür bir yapı kullanılır çünki bu hızlı ve verimlidir.

Nesneleri yaratırken ya da güncellerken akış sistemi hep aynıdır. Sonraki
bölümde Doctrine'nin nasıl zekice veri tabanıda var olan bir kayıt için 
``UPDATE`` ifadesini kullandığını göreceksiniz.

.. tip::

    Doctrine size projeniz içerisinde programsal olarak test yapabilmeniz 
    için bir kütüphane de sağlar. Daha fazla bilgi için 
    :doc:`/bundles/DoctrineFixturesBundle/index` belgesine bakın.

Veritabanından Nesneleri Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Veritabanından bir nesneyi almak çok kolaydır. Varsayalım 
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
kullanabilirsiniz. Repository'nin (ambar), işi sadece entity'leri belirli bir sınıfı
için kullanılacak olan bir PHP sınıfı olarak düşünebilirsiniz. Bir entity sınıfının 
repository sınıfına şu şekilde ulaşabilirsiniz::

    $repository = $this->getDoctrine()
        ->getRepository('AcmeStoreBundle:Product');

.. note::

    ``AcmeStoreBundle:Product`` ifadesi, sınıfın tam adını kullanmak yerine 
    (Örn. ``Acme\StoreBundle\Entity\Product``) Doctrine içerisinde herhangi bir
    yerde bu sınıfı kullanmak için onu kısaca tanımlayan ifadedir.
    Entity sınıfı bundle'ınız içerisindeki ``Entity`` isim uzayında olduğu
    sürece bu çalışacaktır.

Bir kez repository'niz olursa aşağıda sıralanan tüm faydalı metodlara
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

    // fiyata göre sıralı bir şekilde tüm ürünleri getir.
    $product = $repository->findBy(
        array('name' => 'foo'),
        array('price' => 'ASC')
    );

.. tip::

   Herhangi bir sayfayı ekrana bastığınızda ne kadar sorgunun çalıştığını
   görmek isterseniz web debug araç çubuğunun sağ alt köşesine bakın.

    .. image:: /images/book/doctrine_web_debug_toolbar.png
       :align: center
       :scale: 50
       :width: 350

    Eğer ikon'a tıklarsanız profiler açılacak ve size ne kadar sorgunun
    yapıldığını açıkça gösterecektir.

Bir Nesneyi Güncellemek
~~~~~~~~~~~~~~~~~~~~~~~

Doctrinden bir nesneyi aldınız mı güncellemek çok kolaydır.
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

Güncellenen nesne şu üç adımla oluşur:

1. Doctrine üzerinden nesneyi al;
2. nesneyi değiştir;
3. entity manager üzerinden ``flush()``metodunu çağır.

``$em->persist($product)``  metodunun çağırılmasının gereksiz olduğuna dikkat edin.
Bu  metodun yeniden çağırımı Doctrine'e sadece ``$product`` nesnesini yönet
ya da izle demektir. Bu durumda ``$product`` nesnesini Doctrine üzerinden aldığınız
için bu zaten yönetilebilir duruma gelmiştir.Çağırılmasına gerek yoktur.

Bir Nesneyi Silmek
~~~~~~~~~~~~~~~~~~

Bir nesneyi silmek de çok benzer ancak entity manager üzerinden bu sefer
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
sorguları nasıl çalıştırdığını artık gördünüz::

    $repository->find($id);
    
    $repository->findOneByName('Foo');


Elbette Doctrine ayrıca daha karmaşık sorgular için Doctrine Sorgu Dili
adındaki (DQL) bir araçla bu sorguları yazmanıza olanak sağlar.DQL, SQL
diline benzer olarak sadece SQL'deki gibi tablodaki satırlar için sorgular
yazmaktan farklı olarak (Örn: ``product``) bir entity sınıfı ya da birden fazla
entity sınıfı için sorgu yazmanız gereklidir. (Örn ``Product``)

Doctrine üzerinde sorgu yazarken iki seçeneğiniz bulunmaktadır. Birincisi
saf Doctrine sorguları yazmak, diğeri ise Doctrine Sorgu Üreteci'ni (Query Builder)
kullanmaktır.

Nesneleri DQL ile Sorgulamak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Farzedin ki ürünlerinizi sorgulamak istiyorsunuz ancak sadece fiyatı ``19.99``
dan büyük olanları sorgulamak istiyorsunuz ve bunlar en ucuzdan en pahalıya 
doğru sıralanacak şekilde listelenecek. Bir controller içinden şunu yapın::

    $em = $this->getDoctrine()->getEntityManager();
    $query = $em->createQuery(
        'SELECT p FROM AcmeStoreBundle:Product p WHERE p.price > :price ORDER BY p.price ASC'
    )->setParameter('price', '19.99');
    
    $products = $query->getResult();

Eğer SQL 'de rahat ediyorsanız DQL üzerinde daha da rahat hissedeceksiniz. Buradaki
en büyük fark, veri tabanı üzerindeki sorgunuzu satırlar olarak düşünmek yerine
"nesneler" olarak düşünmeniz gerekliliğidir. Bu yüzden 

	select *from*  ``AcmeStoreBundle:Product``

ifadesine ``p`` aliası (takma adı) verilmiştir. 
``getResult()`` Metodu sonuçları bir dize (array) halinde döndürür. Eğer sadece
bir nesne dönüşü için sorguladıysanız bu durumda ``getSingleResult()`` metodunu
bu metodun yerine kullanabilirsiniz::

    $product = $query->getSingleResult();

.. caution::

    ``getSingleResult()`` metodu herhangi bir sonuç dönmemesi halinde bir 
    ``Doctrine\ORM\NoResultException`` istisnası ve eğer birden fazla sonuç döndüyse
    ``Doctrine\ORM\NonUniqueResultException`` istisnası atar. Eğer bu metodu
    kullanırsanız mutlaka bu işlemi try catch içerisine alarak sadece bir sonucun
    dönmesi durumunu kontrol etmeniz gerekir (eğer sorguladığınız herhangi birşeyin
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
    içerisinde kullanmak her zaman iyi bir yöntemdir:
    
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
hemde daha güzel bir şekiklde halledebilirsiniz. Eğer bir IDE kullanıyorsanız aynı zamanda
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


``QueryBuilder`` nesnesi sorgunuzu üretebilmek için tüm metodları içerir.
``getQuery()`` metodunun çağırılması ile sorgu üreteci önceki bölümde gösterildiği gibi
normal bir ``Query`` nesnesi çevirir.
Doctrine'nin Sorgu Üreteci (Query Builder) hakkında daha fazla bilgi almak
için `Sorgu Üreteci`_  dökümanına başvurun.

Özel Ambar(Repository) Sınıfları
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Önceki bölümlerde controller içerisinden karmaşık sorguları üretmeye başlamıştınız.
Ancak bu sorguları test etmek ve yeniden kullanmak için bunları izole ederek entity'nizin 
kullanabileceği özel bir repository (ambar) sınıfı içerisine almak ve 
sorguları mantıksal olarak metodlara atamak daha güzel bir fikirdir.

Bunu yapmak için entity sınıfınızda repository(ambar) sınıfınızın ismini
eşleştirme (mapping) tanımlamanız içerisine eklemeniz gereklidir.

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

    Repository içerisinden entity manager'a  ``$this->getEntityManager()``
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
------------------------------

Varsayalım ki uygulamanızdaki ürünlerin tamamı sadece bir "kategori" 'ye bağlı.
Bu durumda ``Product`` nesnesinin ``Category`` ile ilişkilendirilmiş bir
``Category`` nesnesine ihtiyacınız olacaktır.``Category`` entity'sini yaratmaya başlayalım.
Sizinde bildiğiniz gibi nihayetinde sınıfın Doctrine tarafından veritabanına yazılması
gereklidir. Bırakın bu sınıfı Doctrine sizin için yaratsın.

.. code-block:: bash

    php app/console doctrine:generate:entity --entity="AcmeStoreBundle:Category" --fields="name:string(255)"

Bu işlem ``id`` alanı ve bir ``name`` alanı ve ilgili getter ve setter fonksiyonlarına
sahip olan bir ``Category`` entity'sini sizin için yaratır.

Eşleştirilen Metadatalarda İlişkiler
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``Category` ve ``Product`` entity'lerini ilişkilendirmek için ``Category``
sınıfında bir ``products`` değişkeni yaratalım:

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



Öncelikle bir ``Category`` nesnesi birden falza ``Product`` nesnesi ile ilişkili
olduğunda, bir ``products`` dize değişkeni ilgili ``Product`` nesnelerini barındıracaktır.
Aslında bu Doctrine tarafından ihtiyaç duyulan bir şey değildir ancak bunu
uygulama içerisinde her ``Category`` için ``Product`` nesnelerini bir dize(array)
içerisinde tutmak daha mantıklıdır.

.. note::

    Kod içerisindeki ``__construct()`` metodu Doctrine'nin ``$products`` 
    değişkenini bir ``ArrayCollection`` objesi haline getirmesi için gerekli 
    olduğundan dolayı önemlidir.
    Bu nesne tamamen bir dize değişkeni gibi davranırken bazı esnekliklerde
    sağlar. Eğer bu nesne ile rahat edemezseniz endileşenmeyin. Bu sadece
    bir ``dize`` (array) değişkendir ve bununla ileride daha rahat edeceksiniz.

.. tip::

   Dekoratör içerisindeki targetEntity değeri gerçerli bir namespace'e sahip
   olan bir entity'i işaret eder. Başka sınıf ya da bundle içerisindeki entity ile
   ilişkiyi düzenlemek için targetEntity içerisine namespace değerini tam olarak
   girmeniz gereklidir.

Sonra, her ``Product`` sınıfı sadece bir ``Category`` nesnesi ile ilişkili
olabileceğininden ``Product`` sınıfı içerisine bir ``$category`` değişkeni
eklemek gerekecektir.

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


Sonuç olarak ``Category`` ve ``Product`` nesnelerine bir değişken
eklediniz. Şimdi Doctrine'e eksik olan getter ve setter metodlarını yaratmasını
söyleyin:

.. code-block:: bash

    php app/console doctrine:generate:entities Acme

Doctrine metadata'sını bir an için yoksayın. Şimdi ``Category`` ve ``Product``
adında doğal olarak birden-çoka ilişkisi ile bağlı iki sınıfınız var. 
``Category`` sınıfı ``Product`` nesnelerini bir array içerisinde tutar ve
``Product`` nesnesi sadece bir ``Category`` nesnesi tutar. Diğer bir ifade 
ile sınıflarımızı ihtiyacımız doğrultusunda mantıklı bir şekilde yaratmış olduk.
Aslında veri veritabanına yazılması gerekliliği her zaman ikincil bir konudur.

Şimdi ``Product`` nesnesi altındaki ``$category`` değişkeninde tutulan
metadata'ya bakalım. Buradaki ilgi, doctrine'e ``Category`` sınıfı ile ilişkili
olduğunu ve kategori kaydının ``id`` bilgisini ``product`` tablosunun 
``category_id`` alanında saklanmasını söyler.
Diğer bir ifade ile ilgili ``Category`` nesnesi ``$category`` nesnesine
tutulacak ancak arka planda Doctrine kategorinin id değerini, ``product``
tablosunun ``category_id`` alanında tutacaktır.

.. image:: /images/book/doctrine_image_2.png
   :align: center


``Category`` sınıfının altındaki ``$products`` değişkeninde tutulan metadata
bilgisi daha az önemlidir ve basitçe Doctrine ``Product.category`` tablosuna
bakmasını söyler.

Devam etmeden önce Doctrine 'e yeni bir ``category`` tablosunu, 
``product.category_id`` sütununu ve yeni bir foreign key yaratmasın
söylediğinizden emin olun:

.. code-block:: bash

    php app/console doctrine:schema:update --force

.. note::

    Bu işlem sadece geliştirme ortamında kullanılır. Sistematik olarak
    ürün veritabanınızda daha güçlü bir metod istiyorsanız 
    :doc:`Doctrine migrations</bundles/DoctrineMigrationsBundle/index>` 
    kısmını okuyun.

İlişkili Entity'leri Kayıt Etmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şimdi uygulamadaki koda bakalım. Bir kontroller içinde olduğunuzu düşünün::

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

Şimdi ``category`` ve ``product`` tablolarına bir satır eklendi.
Eklenen ürünün ``product.category_id`` sütunu değeri, eklenen 
yeni kategorinin  yeni ``id`` alanının değerine göre belirlendi. 
Doctrine bu ilişkileri sizin için veritabanına otomatik olarak yazar.

İlişkili Nesneleri Almak
~~~~~~~~~~~~~~~~~~~~~~~~

İlişkilendirilmiş nesneleri çekmek istediğinizde akış önceden
yapıtığınız çekme işine çok benzer. Önce ``$product`` nesnesini çek sonra ilgili 
``Category`` 'ye ulaş::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $categoryName = $product->getCategory()->getName();
        
        // ...
    }

Bu örnekte ilk sorgu ürünün id'sine uygun ``Product`` nesnesi için.
Bu sorgu için sonuçlar *sadece* ürün verisi ve ``$product`` nesnesinin 
bu dönen veri ile birleştirilmiş (hydrate) şeklidir. Sonra ``$product->getCategory()->getName()``
metodunu çağırdığınızda Doctrine sessizce ``Product`` ile ilişkili ``Category`` 'yi bulmak
için ikinci sorguyu çalıştırır. Bu işlem ``$category`` nesnesini sizin için hazırlar ve
döndürür.


.. image:: /images/book/doctrine_image_3.png
   :align: center

Burada aslında önemli olan şey sizin ürünün ilişkili olduğu kategoriye ulaşmanızdan
çok, kategori verisinin siz kategoriyi sormadan gerçekte getirilmemesidir
("lazily loaded").

Diğer bir yönden de sorgu yapabilirsiniz::

    public function showProductAction($id)
    {
        $category = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Category')
            ->find($id);

        $products = $category->getProducts();
    
        // ...
    }

Bu durumda yine aynı şeyler olur. İlk sorgunuz bir ``Category`` nesnesi döndürür ve
Doctrine ilgili ``Product`` 'ları getirmek üzere ikinci sorguyu çalıştırır önce/eğer
siz bunu sordu iseniz. (Örn: ``->getProducts()`` 'ı çağırdığınızda).
``$products`` değişkeni verilen ``Category`` nesnesinin ilgili ``Product`` nesnesindeki
``category_id`` alanını eşleyen tüm verilerini bir dize değişkeni içerisinde döndürür.

.. sidebar:: İlişkiler ve Proxy Sınıfları

    Bu nesnenin ihtiyaç halinde bilgilerinin getirilmesi "lazy loading" mümkündür.
    Çünkü gerektiği zaman, Doctrine gerçek nesnenin yerine bir "proxy" nesnesi
    döndürecektir.Aşağıdaki örneğe yeniden bakalım::
    
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->find($id);

        $category = $product->getCategory();

        // prints "Proxies\AcmeStoreBundleEntityCategoryProxy"
        echo get_class($category);

    Bu proxy nesnesi gerçek ``Category`` nesnesinden türetilmiştir ve
    hem onun gibi gözükür hemde aynen onun gibi davranır.
    Proxy nesnesinin kullanılmasının farkı Doctrine gerçek ``Category``
    verisinin gerçekten ihtiyacınız olduğunda çalışması üzere bekletebilmesidir.
    (Örn: ``$category->getName()`` metodunu çağırana kadar.)

	Tüm proxy sınıfları Doctrine tarafından yaratılır ve cache klasöründe
	saklanır. Belki hiç ``$category`` nesnesini br proxy nesnesi olarak
	kullanmayacak olabilirsiniz ancak bunu akılda tutmak önemlidir.

	Sonraki kısımda tüm ürün ve kategori verisini getirdiğinizde (bir *join*
	aracılığı ile ) Doctine ``Category`` nesnesinin değerini bu nesnenin
	bilgilerini alma ihtiyacı hissedilene kadar (lazy loading)
	*true* döndürecektir.

İlgili Kayıtlarda Join
~~~~~~~~~~~~~~~~~~~~~~
Yukarıdaki örneklerde bir tanesi orijinal nesne için (Örn : ``Category``)
bir taneside ilgili nesne(ler) için (Örn:``Product``) iki adet sorgu yapılıyordu.

.. tip::

    İstek esnasında çalıştırılan tüm sorguları web debug toolbar'dan
    görebileceğinizi Hatırlayın.

Elbette eğer önceden iki nesneye ulaşmak istediğimizde ikinci sorgunun join
sorgusu esnasında çalıştırılmayacağını bilmelisiniz. ``ProductRepository`` 
sınıfına şu metodu ekleyin::

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


Şimdi bu metodu controller içerisinde ``Product`` nesnesini ve ilgil
``Category`` nesnesini bir sorguda sorgulayabilmek için kullanabilirsiniz::

    public function showAction($id)
    {
        $product = $this->getDoctrine()
            ->getRepository('AcmeStoreBundle:Product')
            ->findOneByIdJoinedToCategory($id);

        $category = $product->getCategory();
    
        // ...
    }    

Birliktelikler İçin Daha Fazla Bilgi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Bu kısım da genel entity birliktelik tiplerinden olan birden - çoka (one-to-many)
ilişkisine giriş yapılmıştır. Diğer birliktelik tipleri hakkında daha fazla 
bilgi almak ve daha ileri düzey örnekler için (Örn : ``one-to-one`` (birebir) , 
``many to many`` (çoktan çoka)) Doctrine'nin `Birliktelik Eşleme Belgesi`_. 'ne
bakın. 

.. note::

    Eğer belirteçleri(annotations) kullanıyorsanız tüm belirteçlerin önüne
    Doctrine resmi belgesinde olmamasına rağmen , ``ORM\`` etiketini 
    koymalısınız (Örn. ``ORM\OneToMany``). 
    Ayrıca ``ORM`` belirtecini kullanabilmeniz için ``use Doctrine\ORM\Mapping as ORM;``
    deyimini de kullanmanız gerekecektir.
    

Konfigürasyon
-------------

Doctrine'in muhtemelen pek çok seçeneğini ayarlamaya ihtiyaç duymayacak olmanıza rağmen 
oldukça konfigüre edilebilirdir. Doctrine'nin nasıl konfigüre edilebileceği
hakkındaki belgeyi :doc:`referans belgeleri</reference/configuration/doctrine>` 
kısmında bulabilirsiniz.


LifeCycle Çağrıları (LifeCycle Callbacks)
----------------------------------------------

Bazen bir entity insert edildiğinde, güncellendiğinde ya da silindiğinde
bu hareket gerçekleşmeden önce ya da sonra bir şey yapmaya ihtiyaç duyarsınız.
Bu tipteki hareketler "lifecycle" çağrıları olarak adlandırılırlar. Bu çağrı metodları
bir entity 'nin farklı lifecycle durumlarında çalıştırılırlar (Örn : entity 
insert edildiğinde güncellendiğinde, silindiğinde vs.. ).

Eğer eşleme verileri için (metadata) belirteçleri (annotations) kullanıyorsanız,
lifecycle çağrılarını aktifleştirmeniz gerekir. Eğer YAML ya da XML'i eşleştirme
için kullanıyorsanız bu gerekli değildir:

.. code-block:: php-annotations

    /**
     * @ORM\Entity()
     * @ORM\HasLifecycleCallbacks()
     */
    class Product
    {
        // ...
    }

Şimdi Doctrine herhangi bir lifecycle olayı için herhangi bir metodun 
çalıştılmasını söyleyebilirsiniz. Örneğin varsayalım bir entity ilk kez 
veritabanına yazılıyorsa (Örn : insert ) ``created`` (yaratılma) sütununa
geçerli tarihi de yazalım:

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

    Yukarıdaki örnekler (burada gözükmesede) entity sınıfı içinde 
    sizin ``created`` adında bir değişkeni (property) yarattığınızı varsaymıştır.

Şimdi,entity ilk kez yazılmadan hemen önce, Doctrine otomatik olarak ``created``
alanına geçerli tarihi yazacak olan metodu çağıracaktır.

Bu aşağıdaki gibi diğer lifecycle olaylarına da uygulanabilir:

* ``preRemove``
* ``postRemove``
* ``prePersist``
* ``postPersist``
* ``preUpdate``
* ``postUpdate``
* ``postLoad``
* ``loadClassMetadata``

Genel olarak bu lifecycle olaylarının  ve çağrılarının ne anlama geldiği
konusunda daha fazla bilgi için Doctrine'nin  `Lifecycle Olayları belgesi`_ 
'ne bakın.


.. sidebar:: Lifecycle Çağrıları ve Olay Dinleyicileri (Event Listeners)
	
	``setCreatedValue()`` metodunun herhangi bir argüman almadığına dikkat edin.
	Bu life cycle cağrılarında her zaman genel geçer olan bir şeydir. Lifecycle
	cağrıları entity içerisindeki veriyi değiştiren metodlarla birlikte ilişkili
	olan basit metodlar olmalıdır. (örn : alana değer atamak/yaratmak/güncellemek,
	yapışkan (slug) değer yaratmak gibi)
    
    Eğer log tutmak e-posta göndermek gibi daha ağır işler yapmak istiyorsanız,
    bu durumda olay dinleyici (event listener) ya da olaya katılım yapan (subscriber)
    bir dışsal sınıf içerisinde ne istiyorsanız yapabilirsiniz. Daha fazla bilgi için
    :doc:`/cookbook/doctrine/event_listeners_subscribers` belgesine bakın.
    

Doctrine İlaveleri (Extensions): Timestampable, Sluggable, vs.
--------------------------------------------------------------

Doctrine oldukça esnektir ve entity'leriniz içerisinde sürekli tekrarlanan
olayları basitçe çözebilmek için bir çok 3. parti ilave (extension) ile birlikte gelir. 
Bunlar *Sluggable*, *Timestampable*, *Loggable*, *Translatable*,
ve *Tree* gibi şeyleri kapsar.

Bu extension'ları nasıl kullanacağınız konusunda daha fazla bilgi için 
tarif kitabındaki :doc:`Genel Doctrine extension'larının kullanımı </cookbook/doctrine/common_extensions>` 
adlı makaleyi okuyun.

.. _book-doctrine-field-types:

Doctrine Alan Tipleri (Field Type) Başvurusu
---------------------------------------------

Doctrine çok sayıda alan tipi ile birlikte gelir. Bunların herbirisi 
veritabanınız da hangi tipte sütun kullandıysanız bunlar için bir PHP
veri tipi ile eşleşir. Doctrine'de desteklenen tipler şunlardır:

* **Stringler**

  * ``string`` (kısa stringler için kullanılır)
  * ``text`` (büyük stringler için kullanılır)

* **Sayılar**

  * ``integer``
  * ``smallint``
  * ``bigint``
  * ``decimal``
  * ``float``

* **Tarih ve Zaman** (PHP de bunun için `DateTime`_  nesnesi kullanılır)

  * ``date``
  * ``time``
  * ``datetime``

* **Diğer Tipler**

  * ``boolean``
  * ``object`` (serileştirilmiş ve ``CLOB`` alanı içerisinde saklanmış)
  * ``array`` (serileştirilmiş ve ``CLOB`` alanı içerisinde saklanmış)

Daha fazla bilgi için Doctrine'nin `Eşleştirme Tipleri Belgesi`_ 'ne bakın.

Alan Özellikleri
~~~~~~~~~~~~~~~~

Her alana bazı özellikler uygulanabilir. Uygulanabilen bu özellikler
``type`` (varsayılan değeri ``string``), ``name``, ``length``, ``unique``
ve ``nullable`` özelliklerinden oluşur. Bir kaç örnek yapalım:

.. configuration-block::

    .. code-block:: php-annotations

        /**
         * Bir string alanı 255 karakterden oluşur ve boş olamaz
         * ("type", "length" ve *nullable* özelliklerinin varsayılan değerlerini yansıtmaktadır.)
         * 
         * @ORM\Column()
         */
        protected $name;
    
        /**
         * Bir string alanının uzunluğu 150 olacak ve veri "email_address" sütununa 
         * başka satırlarda tekrarlanmayacak şekilde (unique) yazılacak.
         *
         * @ORM\Column(name="email_address", unique=true, length=150)
         */
        protected $email;

    .. code-block:: yaml

        fields:
            # Bir string alanı 255 karakterden oluşur ve boş olamaz
            # ("type", "length" ve *nullable* özelliklerinin varsayılan değerlerini yansıtmaktadır.)
            # type niteliği yam içerisinde zorunludur.
            name:
                type: string

            # Bir string alanının uzunluğu 150 olacak ve veri "email_address" sütununa 
            # başka satırlarda tekrarlanmayacak şekilde (unique) yazılacak.
            email:
                type: string
                column: email_address
                length: 150
                unique: true

.. note::

    Daha pek çok özellik burada listelenmemiştir. Daha Fazla bilgi için
	Doctrine'nin `Değişken Eşleme belgesi`_ 'ne bakın.

.. index::
   single: Doctrine; ORM Konsol Komutları
   single: CLI; Doctrine ORM

Konsol Komutları
----------------

Doctrine2 ORM entegrasyonu ``doctrine`` başlığı altında bazı konsol komutları sunar.
Komut listesini görmek için konsoldan herhangi bir arguman vermeden şu
komutu çalıştırın:

.. code-block:: bash

    php app/console

Listelenen komutlar arasından ilgili olanlar ``doctrine`` ön eki ile
başlayanlardır. Bu komutlar (ya da diğer Symfony komutları) hakkında
daha fazla bilgi almak için ``help`` komutunu kullanın. Örneğin
``doctrine:database:create`` komutu hakkında daha fazla bilgi almak için
şunu çalıştırın:

.. code-block:: bash

    php app/console help doctrine:database:create

Bazı dikkat çekici ya da ilginç işlevler şunlardır:

* ``doctrine:ensure-production-settings`` - çalışma ortamınızın ürün (production)
  ortamı için etkinliğini kontrol eder. bu herzaman ``prod`` ortamında çalıştırılmalıdır:
  
  .. code-block:: bash
  
    php app/console doctrine:ensure-production-settings --env=prod

* ``doctrine:mapping:import`` - Doctrine'e var olan bir veritabanını analiz
  edip eşleme bilgisi yaratmasına olanak verir. Daha fazla bilgi için 
  :doc:`/cookbook/doctrine/reverse_engineering` belgesine bakın.

* ``doctrine:mapping:info`` - Doctrine tarafından eşleme sırasında basit hataları ya da 
  entity içerisindeki sorunları listeler.
  
* ``doctrine:query:dql`` ve ``doctrine:query:sql`` - komut satırından direkt olarak
  DQL ya da SQL çalıştırılmasına olanak verir. 

.. note::

   Data Fixture'larını yüklenebilmesi için ``DoctrineFixturesBundle`` bundle'ının
   yüklü olması gerekir. Bunu nasıl yapağınızı öğrenmek için ":doc:`/bundles/DoctrineFixturesBundle/index`"
   belgesini okuyun.

Özet
-----

Doctrine ile nesnelerinize odaklanabilir ve bu nesnelerin uygulamanızda 
ne kadar kullanılabilir olduğunu görerek veritabanınıza verinin yazılma 
endişesini daha sonra yaşarsınız.Bu Doctrine'nin verileri, PHP nesnelerinde 
tutmasına olanak sağlaması ve eşleştirme metadata bilgilerinin nesneler 
ile veritabanının ilgili tablosu arasında eşleştirmesiyle mümkün olmaktadır.

Doctrine basit bir düşünce etrafında dönmesine rağmen oldukça güçlü,
karmaşık sorguları yaratma, nesleleri veritabanına yazma durumunda
ortaya çıkan farklı lifecycle olaylarına karışmaya imkan sağlar.

Doctrine hakkında daha fazla bilgi için :doc:`tarif kitabı</cookbook/index>` 
'nın *Doctrine* başlığı altındaki şu makalelere bakın:

* :doc:`/bundles/DoctrineFixturesBundle/index`
* :doc:`/cookbook/doctrine/common_extensions`

.. _`Doctrine`: http://www.doctrine-project.org/
.. _`MongoDB`: http://www.mongodb.org/
.. _`Basit Eşleme Belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html
.. _`Sorgu Üreteci`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/query-builder.html
.. _`Doctrine Sorgu Dili`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/dql-doctrine-query-language.html
.. _`Birliktelik Eşleme Belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/association-mapping.html
.. _`DateTime`: http://php.net/manual/en/class.datetime.php
.. _`Eşleştirme Tipleri Belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#doctrine-mapping-types
.. _`Değişken Eşleme Belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#property-mapping
.. _`Lifecycle Olayları belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/events.html#lifecycle-events
.. _`Ayrılmış SQL anahtar kelimeleri belgesi`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#quoting-reserved-words
.. _`Kalıcı sınıflar`: http://docs.doctrine-project.org/projects/doctrine-orm/en/2.1/reference/basic-mapping.html#persistent-classes
