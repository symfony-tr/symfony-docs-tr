.. index::
   single: Propel

Veritabanları ve Propel
=======================

Hadi kabul edin. En sık ve cebelleşilen görev, veritabanına verileri yazmak 
ve onları veri tabanından okumayı kapsayan görevlerdir. Smfony2 herhangi bir
ORM ile birlikte entegre gelmez ancak Propel entegrasyonu kolaydır.
Başlamak için `Symfony2 ile birlikte çalışmak`_ belgesini okuyun.

Basit Bir Örnek : Bir Ürün
--------------------------

Bu kısımda bir veri tabanı konfigüre edecek, bir ``Product`` nesnesi yaratacak,
onu veri tabanına yazacak ve geri çağıracaksınız.

.. sidebar:: Örnek ile kodlamak

    Eğer bu bölümdeki örneği bir örnek ile takip etmek istiyorsanız 
    ``php app/console generate:bundle --namespace=Acme/StoreBundle`` 
    konsol komutu yardımı ile bir ``AcmeStoreBundle`` yaratın.

Veritabanını Konfigüre Etmek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Başlamadan önce bir veritabanı bilgisini konfigüre etmeniz gerekli. Kural
olarak bu bilgi genellikle ``app/config/parameters.ini`` dosyasındadır:

.. code-block:: ini

    ;app/config/parameters.ini
    [parameters]
        database_driver   = mysql
        database_host     = localhost
        database_name     = test_project
        database_user     = root
        database_password = password
        database_charset  = UTF8

.. note::

    ``parameters.ini`` dosyası içerisinden parametreleri konfigüre etmek sadece
    bir uygulama kuralıdır. Propel için bu parametreler ana konfigürasyon
    dosyası üzerinden de ayarlanabilir:

    .. code-block:: yaml

        propel:
            dbal:
                driver:     %database_driver%
                user:       %database_user%
                password:   %database_password%
                dsn:        %database_driver%:host=%database_host%;dbname=%database_name%;charset=%database_charset%

Şimdi Propel veri tabanınızı tanıyor ve Symfony2 sizin için bir 
veritabanı yaratabilir:

.. code-block:: bash

    php app/console propel:database:create

.. note::

    Bu örnekte ``default`` adında bir bağlantıyı ayarladınız. Eğer
    bir bağlantıdan daha fazlasını ayarlamak istiyorsanız, 
    `PropelBundle konfigürasyon kısmını <working-with-symfony2.html#project_configuration>`_ 
    okuyun.

Bir Model Sınıfı Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~

Propel Dünyasında ActiveRecord sınıfları, sınıfların Propel tarafından bazı iş
mantıkları ile üretildikleri için **model** olarak adlandırılırlar.

.. note::

    Symfony2 ile Doctrine2 kullananlar için **model** , **entity** terimi
    ile eşanlamlıdır.

Varsayalım, Ürünleri listelemesi gereken bir uygulama geliştiriyorsunuz.
Öncelikle ``AcmeStoreBundle`` 'ınız içindeki ``Resources/config`` dizini
altında bir ``schema.xml`` dosyası yaratın.

.. code-block:: xml

    <?xml version="1.0" encoding="UTF-8"?>
    <database name="default" namespace="Acme\StoreBundle\Model" defaultIdMethod="native">
        <table name="product">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
            <column name="price" type="decimal" />
            <column name="description" type="longvarchar" />
        </table>
    </database>

Model'i Geliştirmek
~~~~~~~~~~~~~~~~~~~

``schema.xml`` dosyasını yarattıktan sonra ,Şu komutu çalışlırarak model'inizi 
yaratın:

.. code-block:: bash

    php app/console propel:model:build

Bu uygulamanızı geliştirirken ``AcmeStoreBundle`` bundle içerisindeki ``Model/``
dizinine ihtiyacınız olan sınıfları çabucak oluşturacaktır.

Veritabanı Tabloları/Şemaları Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Şimdi kullanılabilir ``Product`` sınıfına sahipsiniz ve ne ihtiyacınız var
ise veritabanına yazabilirsiniz. Elbette ilgili ``product`` tablonuz 
veritabanınız içerisinde yok. Çok şükür ki Propel otomatik olarak uygulamanız
içerisinde bilinen tüm modeller için veritabanı tablolarını yaratabilir.
bunu yapmak için şunu çalıştırın:

.. code-block:: bash

    php app/console propel:sql:build

    php app/console propel:sql:insert --force

Veritabanınız şema içerisinde belirtien sütünlarla eşleşen tam fonksiyonlu
bir ``product`` tablosuna sahiptir.

.. tip::

    Son üç komutu aşağıdaki şekilde birleştirerek de kullanabilirsiniz:
    ``php app/console propel:build --insert-sql``.

Veritabanına Nesneleri Yazmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şimdi ``Product`` nesneniz ve ilgili ``product`` tablonuz var, veritabanına
veri yazmaya hazırsınız. Controller içerisinden bu oldukça basittir. Bundle'ın
``DefaultController`` 'ına şu satırları ekleyin::

    // src/Acme/StoreBundle/Controller/DefaultController.php
    use Acme\StoreBundle\Model\Product;
    use Symfony\Component\HttpFoundation\Response;
    // ...

    public function createAction()
    {
        $product = new Product();
        $product->setName('A Foo Bar');
        $product->setPrice(19.99);
        $product->setDescription('Lorem ipsum dolor');

        $product->save();

        return new Response('Created product id '.$product->getId());
    }

Bu bir parça kod içerisinde ``$product`` ile nesneyi örneklediniz ve çalıştınız.
``save()`` metodunu çağırdığınızda veritabanına veriyi yazarsınız. Başka
bir servise gerek olmadan nesne kendisini nasıl yazacağını bilir.

.. note::

    Eğer bunu bir örnek yaparak takip ediyorsanız,bir :doc:`route <routing>`
    yaratarak bu aksiyonun çalışmasını sağlayın.

Veritabanından Nesneleri Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Veritananından bir nesneyi çekmekte oldukça kolaydır. Örneğin, varsayalım
``Product`` nesnesinin ``id`` özelliğine göre bir route tasarladınız::
    
    use Acme\StoreBundle\Model\ProductQuery;
    
    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);
    
        if (!$product) {
            throw $this->createNotFoundException('Bu id ile bir ürün bulunamadı: '.$id);
        }
    
        // $product nesnesini şablona aktarmak gibi bir şeyler yapın
    }

Bir Nesneyi Güncellemek
~~~~~~~~~~~~~~~~~~~~~~~

Propelden bir nesneyi çektiğinizde bunu güncellemek kolaydır. Varsayalım
ürün id'si ile eşleşen bir route 'u ve controller içerisinde ürünü 
güncelleyen bir aksiyonunuz var::
    
    use Acme\StoreBundle\Model\ProductQuery;
    
    public function updateAction($id)
    {
        $product = ProductQuery::create()
            ->findPk($id);
    
        if (!$product) {
            throw $this->createNotFoundException('No product found for id '.$id);
        }
    
        $product->setName('New product name!');
        $product->save();
    
        return $this->redirect($this->generateUrl('homepage'));
    }

Bir nesne güncellemek şu üç aşamadan oluşur:

#. Propel'den nesneyi almak;
#. nesneyi düzenlemek;
#. onu saklamak.

Nesneyi Silmek
~~~~~~~~~~~~~~

Bir nesneyide silmek neredeyse aynıdır ancak bu sefer nesnenin ``delete()``
metodunu çağırmak gerekir:: 

    $product->delete();

Nesneleri Sorgulamak
--------------------
    
Propel basit ve karmaşık sorguları herhangi bir zahmete girmeden üretmeye
yarayan ``Query`` sınıfını sağlar::
    
    \Acme\StoreBundle\Model\ProductQuery::create()->findPk($id);
    
    \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByName('Foo')
        ->findOne();

Mesela fiyatı 19.99'dan büyük ürünleri sorgulamak ve ucuzdan pahalıya doğru 
sıralamak istiyorsunuz. Controller içerisinden şunu yapın::

    $products = \Acme\StoreBundle\Model\ProductQuery::create()
        ->filterByPrice(array('min' => 19.99))
        ->orderByPrice()
        ->find();

Bir satırda ürünlerinizi güçlü nesne yönelimli şekilde aldınız. Symdony2'nin
sağladığı tamamen nesne yönelimli programlama ve aynı felsefeyi dahiyane bir
özetleme katmanı (abstaction layer) sağlayan Propel ile SQL ya da başka 
bir şey için zamanınızı harcamanıza gerek yok.

Eğer bazı sorguları yeniden kullanmak istiyorsanız ``ProductQuery``
sınıfına kendi metodlarınızı da ekleyebilirsiniz::

    // src/Acme/StoreBundle/Model/ProductQuery.php
    
    class ProductQuery extends BaseProductQuery
    {
        public function filterByExpensivePrice()
        {
            return $this
                ->filterByPrice(array('min' => 1000))
        }
    }

Fakat şuna dikkat edin; Propel sizin için pek çok metod yaratır ve
basit bir ``findAllOrderedByName()`` metodu herhangi bir efor
sarfetmeden şu şekilde metodları birleştirerek yazılabilir.

    \Acme\StoreBundle\Model\ProductQuery::create()
        ->orderByName()
        ->find();

İlişkiler/Birleşimler
---------------------

Varsayalım uygulamanızdaki ürünlerin tamamı sadece bir "kategori" altında 
olsun. Bu durumda ``Product`` nesnelerinin ``Category`` nesnesi ile
ilişkide olduğu bir ``Kategori`` nesnesine ihtiyacınız var.

``schema.xml`` İçerisine bir ``category`` tanımlaması ekleyerek başlayın:

.. code-block:: xml

    <database name="default" namespace="Acme\StoreBundle\Model" defaultIdMethod="native">
        <table name="product">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
            <column name="price" type="decimal" />
            <column name="description" type="longvarchar" />
    
            <column name="category_id" type="integer" />
            <foreign-key foreignTable="category">
                <reference local="category_id" foreign="id" />
            </foreign-key>
        </table>
    
        <table name="category">
            <column name="id" type="integer" required="true" primaryKey="true" autoIncrement="true" />
            <column name="name" type="varchar" primaryString="true" size="100" />
       </table>
    </database>

Sınıfları Yaratmak:

.. code-block:: bash

    php app/console propel:model:build

Eğer ürünler veritabanınızda ise onları kaybetmek istemezsiniz. Taşıma(migration)
sayesinde Propel veritabanınızı mevcut verilerinizi kaybetmeden güncelleyebilir.

.. code-block:: bash

    php app/console propel:migration:generate-diff

    php app/console propel:migration:migrate

Veritabanınız güncellendi ve uygulamanızı yazmaya devam edebilirsiniz.

İlişkili Nesneleri Saklamak
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şimdi kodu uygulamada görelim. Mesle controller'iniz içerisinde::

    // ...
    use Acme\StoreBundle\Model\Category;
    use Acme\StoreBundle\Model\Product;
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
            // bu ürünü kategori ile ilişkilendir
            $product->setCategory($category);
    
            // tamamını sakla
            $product->save();
    
            return new Response(
                'Created product id: '.$product->getId().' and category id: '.$category->getId()
            );
        }
    }
Şimdi tek satırda ``category`` ve ``product`` tablolarıba satırlar eklendi.
Yeni ürün için ``product.category.id`` sütünü yeni kategori'nin id no'su
ne ise o değer olarak ayarlandı. Propel bu ilişkiyi veritabanına sizin içn yazar.

İlişkili Nesneleri Çekmek
~~~~~~~~~~~~~~~~~~~~~~~~~

Birleşimli (associated) nesneleri çekmek istediğinizde iş akışı önceden
yaptığınız gibidir. Öncelikle bir ``$product`` nesnesi çek ve ilgili ``Category``
'ye eriş::

    // ...
    use Acme\StoreBundle\Model\ProductQuery;
    
    public function showAction($id)
    {
        $product = ProductQuery::create()
            ->joinWithCategory()
            ->findPk($id);
    
        $categoryName = $product->getCategory()->getName();
    
        // ...
    }

Yukarıdaki örnekte sadece bir sorgu yapıldığına dikkat edin.

Birleşimler Hakkında Daha Fazla Bilgi
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

İlişkiler hakkında daha fazla bilgi almak için veritabanı `İlişkiler`_ 'ine
ayrılmış bölümü okuyun.

Lifecycle Çağrıları
-------------------

Bazen bir olay gerçekleşmeden hemen önce ya da sonra bir nesne 
insert edilmesi, güncellenmesi ya da silinmesi gerekebilir. Aksiyonun
bu tipleri "lifecycle" cağrıları ya da "hook" 'ları olarak adlandırılan
nesnenin farklı aşamalardaki lifecycle metodlarını çalıştırmanız gerekir
(Örn: nesne insert edildiğinde, güncellendiğinde ya da silindiğinde vs..).

Bir hook eklemek için sadece nesneye bir metod eklemeniz gerekir::

    // src/Acme/StoreBundle/Model/Product.php
    
    // ...
    
    class Product extends BaseProduct
    {
        public function preInsert(\PropelPDO $con = null)
        {
            // nesne insert edilmeden önce bir şeyler yap.
        }
    }

Propel aşağıdaki hook'ları destekler:

* ``preInsert()`` yeni bir nesne insert edilmeden önce çalışacak kod
* ``postInsert()``  yeni bir nesne insert edildikten sonra çalışacak kod
* ``preUpdate()`` mevcut nesne güncellenmeden önce çalıştırılacak kod
* ``postUpdate()`` mevcut nesne güncellendikten sonra çalıştırılacak kod
* ``preSave()`` nesne saklanmadan önce çalıştırılacak kod (yeni ya da mevcut)
* ``postSave()`` nesne saklandıktan sonra çalıştırılacak kod (yeni ya da mevcut)
* ``preDelete()`` nesneyi silmeden önce çalıştırılacak kod
* ``postDelete()`` nesneyi sildikten sonra çalıştırılacak kod


Davranışlar
-----------

Propel içerisindeki tüm bundle edilmiş davranışlar Symfony2'den çalışabilir.
Propel davranışlarının nasıl kullanılacağı konusunda daha fazla bilgi almak
için `Davranış referansları kısmı`_ 'na bakın.

Komutlar
--------
Bu konuda daha fazla bilgi almak için `Symfony2'deki Propel Komutları`_ 
kısmını okuyun.

.. _`Symfony2 ile birlikte çalışmak`: http://www.propelorm.org/cookbook/symfony2/working-with-symfony2.html#installation
.. _`İlişkiler`: http://www.propelorm.org/documentation/04-relationships.html
.. _`Davranış referansları kısmı`: http://www.propelorm.org/documentation/#behaviors_reference
.. _`Symfony2'deki Propel Komutları`: http://www.propelorm.org/cookbook/symfony2/working-with-symfony2#commands
