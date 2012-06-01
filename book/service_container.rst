.. index::
   single: Servis Kutusu (Container)
   single: Dependency Injection Kutusu (Container)

Servis Kutusu (Container)
=================

Modern bir PHP uygulaması tamamen nesnedir. Bir nesne e-posta mesajlarını
iletmeyi sağlayıp kolaylaştırır iken, diğeri belki bir bilgiyi veritabanına
yazabilir. Uygulamanızda ürün envamnterinizi yönetecek bir nesne yaratabilir
ya da başka bir nesne 3. parti API'den bir veri işleyebilir. Ana nokta, modern
uygulamalar pek çok şey ile ilgili süreci organize edilmiş pek çok nesne ile
yapmalarıdır. 

Bu bölümde size Symfony2'deki örneklemeye, organize etmeye ve uygulamanızdaki
pek çok nesneyi getirmeye yarayapn özel bir PHP nesnesinden bahsedeceğiz.
Servis Kutusu (container) olarak adlandırılan bu nesne, uygulamanızdaki
yapılandırdığınız nesnelerin standardize edilmesi ve merkezileştirilmesinin
bir yoludur. Bu kutu (container) hayatı kolaylaştırıcı, süper hızlı ve kodu ayrıştıran
, yeniden kodun kullanılabilmesine imkan sağlayan bir mimariyi temsil eder. 

Tüm Symfony2 çekirdek sınıfları kutu (container) kullanmasından dolayı,
Symfony2'de herhangi bir nesneyi nasıl genişletebileceğinizi ve konfigüre
edebileceğinizi öğreneceksiniz.

Geniş bir kapsamda servis kutusu, Symfony2'nin hız ve genişletilebilirlik
özelliklerine en büyük katkıyı yapar.

Sonuç olarak, servis kutusunu konfigüre etmek ve kullanmak kolaydır. Bu 
kısmın sonunda container vasıtası ile rahatça nesnelerinizi yaratabilecek ve
herhangi bir 3.parti bundle nesnelerini özelleştirebileceksiniz. Servis
container 'in iyi kod yazmayı kolaylaştırıp basitleştirmesi ile
daha yeniden kullanılabilir, test edilebilir ve ayrıştırılabilir kod
yazacaksınız.

.. index::
   single: Servis Container; Servis nedir ?

Servis nedir ?
--------------

:term:`Servis` basitçe, Uygulamada "genel"(global) bir dizi işlemi gerçekleştiren herhangibir
PHP nesnesidir. Bilgisayar bilimlerinde bir terim olarak kullanılan bu isim 
bir nesnenin özel bir amaç için yaratılmasını ifade eder (Örn: e-posta
göndermek). Her servis uygulamanız içerisinde ne zaman isterseniz bu özel
işlemi gerçekleştirmenizi sağlar. Bir servis yapmak için özel bir şey 
yapmanıza gerek yok. Sadece bu belirli işlemi gerçekleştirecek kodu
içeren br PHP sınıfı geliştirmelisiniz.

Tebrikler!. Artık bir servisiniz var.

.. note::

    Bir kural olarak, PHP nesnesi uygulamanızda genel olarak kullanılabiliyorsa
    bir servistir. Tek bir ``Mailer`` servisi genel olarak e-posta mesajlarını
    göndermekten sorumluysa bir servistir yoksa çok sayıda ``Message`` nesnesi
    bunu yapıyorsa bu bir servis *değildir* . Aynı şekilde bir ``Product`` nesnesi
    bir servis değildir ancak bir nesne ``Product`` nesnelerini veritabanına
    yazıyorsa bir *servistir* . 

E peki ne farkeder ? "Servis" 'lerin avantajlarını düşündüğümüzde uygulmanızın
her özelliğini küçük parçalar halinde bir dizi servis'e dönüştürecek şekilde
düşünmeye başlarsınız. Her servis sadece bir işi yapmasına rağmen, her servise
kolaylıkla erişebilip bu servislerin özelliklerini istediğiniz her yerde
kullanabilirsiniz. Her servis ayrıca uygulamanızın diğer işlevsel parçalarına
göre daha kolay test edileblir ve konfigüre edilebilir. Bu yaklaşıma
`servis tabanlı mimari`_ denir ve PHP de ya da Symfony2'ye özgü bir şey değildir.
Uygulamanızı bir dizi bağımsız bir servis sınıfları etrafında toplamak 
güvenli ve iyi olarak bilinen nesne yönelimli mimarinin iyi uygulamalarından
birisidir. Bu şekilde yaklaşımlar herhangi bir dil üzerinde iyi bir geliştirci
olmanın anahtarıdır.

.. index::
   single: Servis Kutusu (Container); Nedir ?

Servis Kutusu (Container) Nedir ?
---------------------------------

:term:`Servis Kutusu (Container)` (ya da  *bağımlılık aşılama kutusu 
(dependency injection container)*) basitçe servisin örneğini yöneten
bir PHP nesnesidir. Örneğin varsayalım bizim e-postaları gönderen bir PHP
nesnemiz var. Servis kutusu olmadan ne zaman bu nesne bize lazım olsa
bunu manuel olarak yaratmamız gerekir:

.. code-block:: php

    use Acme\HelloBundle\Mailer;

    $mailer = new Mailer('sendmail');
    $mailer->send('ryan@foobar.net', ... );

Bu yeterince basit. Örneğin hayali ``Mailer`` sınıfı bize e-posta mesajlarını 
gönderecek metodu konfigüre etmemize izin versin (Örn.: ``sendmail``, ``smtp``, vs).
Fakat acaba biz bu posta servisini başka yerde kullanmak istersek? Kuşkusuz
mailer konfigürasyonunu ``Mailer`` objesini her kullanmak istediğimizde 
tekrar tekrar konfigüre etmek istemeyiz. Acaba eğer gerektiğinde ``transport``
'u ``sendmail`` den ``smtp`` ye uygulamamızın istediğimiz herhangi bir yerinde
değiştirmemize ihtiyaç olsaydı ?  Bu durumda uygulamamız içerisindeki her 
``Mailer`` servisini arayıp bulacak ve bunu uygun bir şekilde değiştirmemiz 
gerekecekti.

.. index::
   single: Servis Kutusu (Container); Servisleri Konfigüre Etmek

Kutu (Container) içerisinde Servisleri Yaratmak / Konfigüre etmek
------------------------------------------------------------------

En iyi cevap, ``Mailer`` nesnesini sizin için yaratan bir servis kutusuna
işi bırakmaktır. Bu çalışma sırasında container'in nasıl ``Mailer``
servisi yarattığını *öğretmeliyiz*. Bu, YAML, XML ya da PHP olarak 
tanımlanabilen konfigürasyon üzerinden yapılır:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        services:
            my_mailer:
                class:        Acme\HelloBundle\Mailer
                arguments:    [sendmail]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <services>
            <service id="my_mailer" class="Acme\HelloBundle\Mailer">
                <argument>sendmail</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setDefinition('my_mailer', new Definition(
            'Acme\HelloBundle\Mailer',
            array('sendmail')
        ));

.. note::

    Symfony2 başlarken, servis kutusunu uygulama kongirüasyonuna
    bakarak yaratır(varsayılan olarak ``app/config/config.yml``).
    Bu dosyanın yüklenmesini ortama göre 
    `AppKernel::registerContainerConfiguration()``
    metodu tarafından söyler 
    (Örn : ``dev`` ortamı için ``config_dev.yml`` ya da  ``prod`` ortamı
    için ``config_prod.yml``)

``Acme\HelloBundle\Mailer`` nesnesinin bir örneği şimdi servis kutusu tarafından
mevcut hale getirilmiştir. Kutu'ya (container) herhangi bir geleneksel Symfony2 
controlleri içerisinden ``get()`` kısayol metodu ile erişilebilir:

    class HelloController extends Controller
    {
        // ...

        public function sendEmailAction()
        {
            // ...
            $mailer = $this->get('my_mailer');
            $mailer->send('ryan@foobar.net', ... );
        }
    }

``my_mailer`` servisini container üzerinden sorduğumuzda, container nesneyi
kurar ve geri döndürür. Bu durum servis kutusunun başka büyük bir avantajıdır.
Şöyle ki, bir servis *asla* gerekli olmadığı sürece başlatılmaz. Eğer
bir servisi tanımlayıp hiç bir istek anında kullanmadıysanız servis asla
yaratılmaz. Bu hafızadan yer kazandırır ve uygulamanızın hızını arttırır.
Bunun anlamı ayrıca çok fazla servis tanımlansa bile performans da çok
az ya da sıfır düzeyinde bir düşüşün yaşanmasıdır. Servisler kullanılmadıkları
sürece asla başlatılmazlar.

Başka bir güzel yan ise ``Mailer`` servisi yaratıldığında istek anında sadece
bir örnek (instance) üzerinden servis çalışır. Bu neredeyse tam olarak
ihtiyacınız olan bir davranıştır (daha esnek ve güçlü) fakat daha sonra
servisleri çoklu örneklerde (multiple instances)nasıl kullanılacağının 
konfigürasyonunu da göreceğiz.

.. _book-service-container-parameters:

Servis Parametreleri
--------------------
Yeni servisleri (Örn. nesneler) container aracılığı ile yaratmak
oldukça kolaydır. Parametreler tanımlanan servisleri daha organize ve esnek 
yaparlar:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        %my_mailer.class%
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Sonuç tamamen önceki ile aynı şekildedir -tek fark servisi *nasıl*
tanımladığımızdır-. ``my_mailer.class`` ve ``my_mailer.transport`` 
ifadeleri yüzdelik (``%``) işaretleri arasında tanımlanmıştır.
Container bunların parametre isimleri olduğunu bilir. Bir container,
yapılanma esnasında her parametrenin değerine bakar ve bunları servis
tanımlamasında kullanır.

.. note::

    Parametre ya da argüman içerisindeki yüzde işareti metin değerinin bir 
    parçası ise mutlaka diğer yüzde işaretinden farklı olarak kaçış 
    karakterleri ile ayrı bir şekilde  ifade edilmelidir:
    
    .. code-block:: xml

        <argument type="string">http://symfony.com/?foo=%%s&bar=%%d</argument>


Parametrelerin amacı servise bilgileri göndermektir. Elbette
bir servisin hiç bir parametre olmadan da tanımlanmasında bir hata yoktur.
Ancak parametrelerin bazı avantajları da vardır:

* servisin seçenekleri tek bir ``parameters`` anahtarı altında
  ayrılır ve organize edilir;

* parametre değerleri çoklu servis tanımlamalarında da kullanılabilir

* Bundle içerisinde bir servis yaratılırken (bunu kısaca göreceğiz), 
  parametrelerin kullanımı, uygulamanız içerisinde servisin daha kolay
  özelleştirilmesine olanak sağlar. 

Parametreleri kullanıp kullanmama seçeneği size bağlıdır. Yüksek 
kaliteli 3. parti bundle'lar *daima* container içerisinde saklanan 
servisler için daha ayarlabilir olması açısından parametre kullanırlar.
Ancak uygulamanızdaki servisler için parametrelerin esnekliğine çok da 
ihtiyacınız olmayabilir.

Array(Dize) Parametreleri
~~~~~~~~~~~~~~~~~~~~~~~~~
Parametreler sadece düz metinler olmaktan çok array (dize) şeklinde de olabilir.
XML formatı için type="collection"  niteliğini kullanarak tüm parametreleri
bir dize olarak gösterebilirsiniz::


.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        parameters:
            my_mailer.gateways:
                - mail1
                - mail2
                - mail3
            my_multilang.language_fallback:
                en:
                    - en
                    - fr
                fr:
                    - fr
                    - en

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <parameters>
            <parameter key="my_mailer.gateways" type="collection">
                <parameter>mail1</parameter>
                <parameter>mail2</parameter>
                <parameter>mail3</parameter>
            </parameter>
            <parameter key="my_multilang.language_fallback" type="collection">
                <parameter key="en" type="collection">
                    <parameter>en</parameter>
                    <parameter>fr</parameter>
                </parameter>
                <parameter key="fr" type="collection">
                    <parameter>fr</parameter>
                    <parameter>en</parameter>
                </parameter>
            </parameter>
        </parameters>

    .. code-block:: php

        // app/config/config.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.gateways', array('mail1', 'mail2', 'mail3'));
        $container->setParameter('my_multilang.language_fallback',
                                 array('en' => array('en', 'fr'),
                                       'fr' => array('fr', 'en'),
                                ));


Diğer Container Kaynaklarını Almak (Import)
-------------------------------------------

.. tip::

    Bu kısımda servis konfigürasyon dosyalarını *kaynak* olarak nitelendireceğiz.
    Bir konunun altını çizelim.Pekçok konfigürasyon kaynağı dosyalar 
    şeklinde iken (Örn: YAML,XML,PHP) Symfony2 bu konfigürasyonları 
    herhangi biryerden çağırabildiği için daha esnektir
    (örn: veritabanı ya da başka bir web servisi aracılığı ile).

Servis containerı tek bir konfigürasyon kaynağından yapılandırılır
(varsayılanolarak ``app/config/config.yml``).Diğer tüm servis konfigürasyonları
(Symfony2 çekirdek ve 3.parti bundle konfigürasyonları)
bu dosya içerisine bir ya da daha fazla şekilde aktarılmalıdır. Bu uygulamanızdaki
servisler üzerinde mutlak bir esneklik verir.

Dış servis konfigürasyonlarıda iki şekilde aktarılabilir (import). Birincisi
uygulamalarızda daha sık kuyllandığınız metod olan ``imports`` direktifi.
Sonraki kısımda 3.parti bundlelardan servis konfigürasyonlarını aktarmak konusunda 
daha esnek ve tercih edilen ikinci yöntemi inceleyeceğiz.

.. index::
   single: Servis Container; İçeri aktatmak(imports)

.. _service-container-imports-directive:

``imports`` ile Konfigürasyonları İçeri Aktarmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şimdiye kadar ``my_mailer`` servis container tanımlamasını direkt olarak uygulama
konfigürasyon dosyası içerisinden yaptık (Örn:  ``app/config/config.yml``).
``Mailer`` sınıfının kendisi ``AcmeHelloBundle`` içinde olduğundan dolayı 
``my_mailer`` container tanımlamasını kendi yerinde yapmak daha mantıklı olacaktır.

Öncelikle ``my_mailer`` container tanımlamasını ``AcmeHelloBundle`` içerisindeki
yeni container kaynağına taşıyalım. Eğer ``Resources`` ya da ``Resources/config`` 
klasörleri yok ise bunları yaratmalısınız::

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            my_mailer.class:      Acme\HelloBundle\Mailer
            my_mailer.transport:  sendmail

        services:
            my_mailer:
                class:        %my_mailer.class%
                arguments:    [%my_mailer.transport%]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <parameter key="my_mailer.class">Acme\HelloBundle\Mailer</parameter>
            <parameter key="my_mailer.transport">sendmail</parameter>
        </parameters>

        <services>
            <service id="my_mailer" class="%my_mailer.class%">
                <argument>%my_mailer.transport%</argument>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;

        $container->setParameter('my_mailer.class', 'Acme\HelloBundle\Mailer');
        $container->setParameter('my_mailer.transport', 'sendmail');

        $container->setDefinition('my_mailer', new Definition(
            '%my_mailer.class%',
            array('%my_mailer.transport%')
        ));

Tanımlamanın kendisi değişmedi, sadece konumu değişti. Elbette servis
container'ı yeni kaynak dosyasının konumunu bilmiyor. Çok şükür ki
bunu uygulama konfigürasyon dosyasına ``imports`` anahtarı ile aktarabiliyoruz.

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        imports:
            - { resource: @AcmeHelloBundle/Resources/config/services.yml }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <imports>
            <import resource="@AcmeHelloBundle/Resources/config/services.xml"/>
        </imports>

    .. code-block:: php

        // app/config/config.php
        $this->import('@AcmeHelloBundle/Resources/config/services.php');

``imports`` direktifi uygulamanız içerisindeki servis container konfigürasyon
kaynaklarını başka bir lokasyondan yüklemenize olanak sağlar(genellikle 
bundle'lar üzerinden). Dosyalar için ``resource`` konumu dosyaların tam
yollarını tanımlar. ``@AcmeHell`` şeklindeki özel yazım ise ``AcmeHellBundle``
adındaki bundle'uın yolunu otomatik olarak çözer. Bu size eğer ``AcmeHelloBundle``
bundle'ını başka bir yere taşıdığınızda konumlarının değişmesinden kaynaklanacak
sorunlar için endileşenmemenizi sağlar.

.. index::
   single: Servis Container; Extension(Eklenti) konfigürasyonu

.. _service-container-extension-configuration:

Container Extensionları üzerinden Konfigürasyon Aktarmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 de geliştirme yaparken uygulamanıza özel geliştirdiğiniz bundle'ların
container konfigürasyonlarını aktarırken en çok ``imports`` direktifini kullanırız.
3.Parti bundle konfigürasyonu, Symfony2 çekirdek servisleri dahil, genellikle 
diğer metodu kullanarak yapılan yüklemede uygulamanız daha esnek ve kolay bir 
şekilde konfigüre edilebilir.

Burada gösterildiği şekilde içsel olarak her bundle kendi servisini bu zamana
kadar gördüğümüz şekile çok yakın olarak tanımlar. Yani bir bundle bir ya da 
daha fazla konfigürasyon kaynak dosyasını (genellikle XML) kendisinin parametre
ve servislerini tanımlamak için kullanır.Ancak bu kaynakları direk uygulama 
konfigürasyonundan ``imports`` direktifi ile aktarmak yerine basitçe
bundle içerisinde *servis container extension* çağırılarak bu sizin için yapılır.
Bir servis container extension'u bundle yazarı tarafından iki şeyi başarmak
amacıyla yapılan bir PHP sınıfıdır:

* bundle'ın servis konfigürasyonuna gereken tüm servis container kaynaklarını
  içeri aktarmak 

* daha basit ve kolay anlaşılabilirliği sağladığından bundle, bundle 
  container konfigürasyonundaki düz parametreler ile etkileşime girmez.

Başka bir ifade ile bir servis container extension 'u  bir bundle için
servisleri sizin yerinize konfigüre eder. Ve birazdan göreceğimiz gibi
extension, bundle 'ı konfigüre etmek için düzgün ve yüksek seviyeli
bir arabirim sağlar.

``FrameworkBundle`` örneğine - çekirdek Symfony2 framework bundle - bakalım
Uygulama konfigürasyon dosyasında bulunan aşağıdaki kod ``FrameworkBundle`` 
içerisindeki servis extension container'ini çağırır ::

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            secret:          xxxxxxxxxx
            charset:         UTF-8
            form:            true
            csrf_protection: true
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }
            # ...

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config charset="UTF-8" secret="xxxxxxxxxx">
            <framework:form />
            <framework:csrf-protection />
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
            <!-- ... -->
        </framework>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            'secret'          => 'xxxxxxxxxx',
            'charset'         => 'UTF-8',
            'form'            => array(),
            'csrf-protection' => array(),
            'router'          => array('resource' => '%kernel.root_dir%/config/routing.php'),
            // ...
        ));

Konfigürasyon yorumlandığında container bu extension'u idare edecek olan 
``framework`` direktifini arar. Extension için sorun hangisi ``FrameworkBundle``
'ı temsil eder ve ``FrameworkBundle`` yüklendiğinde hangisi çağırılacak
sorusudur. Eğer ``framework`` anahtarını uygulama konfigürasyon dosyanızdan
tamamen silerseniz çekirdek Symfony2 servisi yüklenemeyecektir. Bu noktada 
kontrol sizdedir. Symfony2 framework asla bir şeyler yaparken bir sihir ya da
sizin kontrolünüz dışında bir şey yapmaz.

Elbette ``FrameworkBundle`` servis container extension'unu daha basit
olarak aktifleştirebilirsiniz. Her extension içsel servisilerin nasıl
tanımlandığı endişesi olmadan kolaylıkla bundle'ın özelleştirmesine
imkan sağlar.

Bu durumda extension ``charset``, ``error_handler``,``csrf_protection``, 
``router`` ve daha fazlasının konfigürasyonuna izin verir. İçsel olarak
``FrameworkBundle`` burada tanımlanan ve konfigüre edilen seçenekleri 
ver belirtilen servisleri kıllanır. Bundle servis containeri nın içerisinde
kolaylıkla özelleştirilebilen tüm ``paremetre`` ve ``servis`` 'lere dikkat eder.
Buna ek olarak pek şom servis container'ı ayrıca seçeneklerde belirttiğiniz
verilerin doğrulamasını yanlış veri tipleri için de yaparlar.

Bundle kurulumu ya da konfigürasyonunda servislerin nasıl kurulup ayarlandığını
görmek için bundle dökümanlarına bakın. Çekirdek bundle'lar için bu dökümanlar
:doc:`Referans Klavuzu</reference/index>` 'nda belirtilmiştir.

.. note::

   Doğal olarak, servis container'ı sadece ``parameters`` , ``services``
   ve ``imports`` direktiflerini tanır. Bunlar dışında kalanları bir 
   servis container extension' u tarafından işlenir.

Eğer kendi bunde'ınız için kullanıcı dostu bir konfigürasyon yapmak istiyorsanız,
":doc:`/cookbook/bundles/extension`"  tarif kitabı reçetesini okuyun.

.. index::
   single: Servis Container; Servisleri İlişkilendirmek

Servisleri İlişkilendirmek (Injecting) 
--------------------------------------

Şimdiye kadar bizim orijinal ``my_mailer`` servisimiz yapıcısı içerisinde 
sadece bir argüman aracılığı ile kolaylıkla konfigüre edilebilen oldukça basit
bir servisti.  İşte container'in gücü, container kendi içerisinde bir ya da daha 
fazla farklı servis'e bağlı olduğu durumlarda ortaya çıkar.

Şimdi bir örnek ile başlayalım. Varsayalım ``NewsletterManager`` adında 
bir adres yığınına email mesajlarımızı göndermek ve hazırlamak işlerini
yöneten yeni bir servisimiz var. Elbette ``my_mailer`` servisi gerçekten
mesajları güzel bir şekilde gönderdiği için ``NewsletterManager`` içerisinde 
mesajları gönderme işinde kullanıyoruz.  Bu işlemi yapan sınıf şu şekilde olabilir::

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function __construct(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Servis container kullanmadan yeni ``NewsletterManager`` 'i controller
içerisinden oldukça kolay bir şekilde yaratabiliriz::


    public function sendNewsletterAction()
    {
        $mailer = $this->get('my_mailer');
        $newsletter = new Acme\HelloBundle\Newsletter\NewsletterManager($mailer);
        // ...
    }

Bu yaklaşım iyi ancak acaba daha sonra ``NewsletterManage`` sınıfı ikinci 
ya da üçüncü bir yapıcı argümana ihtiyaç duyarsa? Acaba kodumuzu refactor
yaparsak ve sınıfın ismini değiştirirsek ne olur ? Bu iki durumda ``NewsletterManager``
sınıfının kullanıldığı her yeri tek tek araştırıp, düzeltmeniz gerekir. 

Elbette servis container'ı bize çok çekici bir seçenek sunmakta::

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@my_mailer]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer"/>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer'))
        ));

YAML içerisinde ``@my_mailer`` özel yazımı container'a ``my_mailer`` adında 
bir servise bakmsını ve bu nesneyi ``NewsletterManager`` nesnesinin yapıcısına
aktarmasını söyler.Ancak bu durumda ``my_mailer`` olarak belirtilen servisin
olması gerekir. Eğer yoksa bir istisna (exception) yaratılacaktır. Bağımlıkları
opsiyonel olarak işaretleyebilirsiniz (Bunu sonraki kısımda göreceğiz).

İlişkilendirmeleri kullanmak bağımsız servisleri birbirleri ile bağımlı hale
getirmede oldukça güçlü araçlardır. Bu örnete ``newsletter_manager`` servisi 
çalışabilmesi için ``my_mailer`` servisine ihtiyaç duymaktadır. Servis container'ı
içerisinde bu bağımlılığı tanımlarken, contaner tüm örneklenen objelerin 
çalışabilir olmasına dikkat eder.

Optional Dependencies: Setter Injection
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Injecting dependencies into the constructor in this manner is an excellent
way of ensuring that the dependency is available to use. If you have optional
dependencies for a class, then "setter injection" may be a better option. This
means injecting the dependency using a method call rather than through the
constructor. The class would look like this::

    namespace Acme\HelloBundle\Newsletter;

    use Acme\HelloBundle\Mailer;

    class NewsletterManager
    {
        protected $mailer;

        public function setMailer(Mailer $mailer)
        {
            $this->mailer = $mailer;
        }

        // ...
    }

Injecting the dependency by the setter method just needs a change of syntax:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...
            newsletter_manager.class: Acme\HelloBundle\Newsletter\NewsletterManager

        services:
            my_mailer:
                # ...
            newsletter_manager:
                class:     %newsletter_manager.class%
                calls:
                    - [ setMailer, [ @my_mailer ] ]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->
        <parameters>
            <!-- ... -->
            <parameter key="newsletter_manager.class">Acme\HelloBundle\Newsletter\NewsletterManager</parameter>
        </parameters>

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <call method="setMailer">
                     <argument type="service" id="my_mailer" />
                </call>
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%'
        ))->addMethodCall('setMailer', array(
            new Reference('my_mailer')
        ));

.. note::

    The approaches presented in this section are called "constructor injection"
    and "setter injection". The Symfony2 service container also supports
    "property injection".

Making References Optional
--------------------------

Sometimes, one of your services may have an optional dependency, meaning
that the dependency is not required for your service to work properly. In
the example above, the ``my_mailer`` service *must* exist, otherwise an exception
will be thrown. By modifying the ``newsletter_manager`` service definition,
you can make this reference optional. The container will then inject it if
it exists and do nothing if it doesn't:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/services.yml
        parameters:
            # ...

        services:
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@?my_mailer]

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/services.xml -->

        <services>
            <service id="my_mailer" ... >
              <!-- ... -->
            </service>
            <service id="newsletter_manager" class="%newsletter_manager.class%">
                <argument type="service" id="my_mailer" on-invalid="ignore" />
            </service>
        </services>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/services.php
        use Symfony\Component\DependencyInjection\Definition;
        use Symfony\Component\DependencyInjection\Reference;
        use Symfony\Component\DependencyInjection\ContainerInterface;

        // ...
        $container->setParameter('newsletter_manager.class', 'Acme\HelloBundle\Newsletter\NewsletterManager');

        $container->setDefinition('my_mailer', ... );
        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(new Reference('my_mailer', ContainerInterface::IGNORE_ON_INVALID_REFERENCE))
        ));

In YAML, the special ``@?`` syntax tells the service container that the dependency
is optional. Of course, the ``NewsletterManager`` must also be written to
allow for an optional dependency:

.. code-block:: php

        public function __construct(Mailer $mailer = null)
        {
            // ...
        }

Core Symfony and Third-Party Bundle Serviss
--------------------------------------------

Since Symfony2 and all third-party bundles configure and retrieve their services
via the container, you can easily access them or even use them in your own
services. To keep things simple, Symfony2 by default does not require that
controllers be defined as services. Furthermore Symfony2 injects the entire
service container into your controller. For example, to handle the storage of
information on a user's session, Symfony2 provides a ``session`` service,
which you can access inside a standard controller as follows::

    public function indexAction($bar)
    {
        $session = $this->get('session');
        $session->set('foo', $bar);

        // ...
    }

In Symfony2, you'll constantly use services provided by the Symfony core or
other third-party bundles to perform tasks such as rendering templates (``templating``),
sending emails (``mailer``), or accessing information on the request (``request``).

We can take this a step further by using these services inside services that
you've created for your application. Let's modify the ``NewsletterManager``
to use the real Symfony2 ``mailer`` service (instead of the pretend ``my_mailer``).
Let's also pass the templating engine service to the ``NewsletterManager``
so that it can generate the email content via a template::

    namespace Acme\HelloBundle\Newsletter;

    use Symfony\Component\Templating\EngineInterface;

    class NewsletterManager
    {
        protected $mailer;

        protected $templating;

        public function __construct(\Swift_Mailer $mailer, EngineInterface $templating)
        {
            $this->mailer = $mailer;
            $this->templating = $templating;
        }

        // ...
    }

Configuring the service container is easy:

.. configuration-block::

    .. code-block:: yaml

        services:
            newsletter_manager:
                class:     %newsletter_manager.class%
                arguments: [@mailer, @templating]

    .. code-block:: xml

        <service id="newsletter_manager" class="%newsletter_manager.class%">
            <argument type="service" id="mailer"/>
            <argument type="service" id="templating"/>
        </service>

    .. code-block:: php

        $container->setDefinition('newsletter_manager', new Definition(
            '%newsletter_manager.class%',
            array(
                new Reference('mailer'),
                new Reference('templating')
            )
        ));

The ``newsletter_manager`` service now has access to the core ``mailer``
and ``templating`` services. This is a common way to create services specific
to your application that leverage the power of different services within
the framework.

.. tip::

    Be sure that ``swiftmailer`` entry appears in your application
    configuration. As we mentioned in :ref:`service-container-extension-configuration`,
    the ``swiftmailer`` key invokes the service extension from the
    ``SwiftmailerBundle``, which registers the ``mailer`` service.

.. index::
   single: Servis Container; Advanced configuration

Advanced Container Configuration
--------------------------------

As we've seen, defining services inside the container is easy, generally
involving a ``service`` configuration key and a few parameters. However,
the container has several other tools available that help to *tag* services
for special functionality, create more complex services, and perform operations
after the container is built.

Marking Serviss as public / private
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When defining services, you'll usually want to be able to access these definitions
within your application code. These services are called ``public``. For example,
the ``doctrine`` service registered with the container when using the DoctrineBundle
is a public service as you can access it via::

   $doctrine = $container->get('doctrine');

However, there are use-cases when you don't want a service to be public. This
is common when a service is only defined because it could be used as an
argument for another service.

.. note::

    If you use a private service as an argument to more than one other service,
    this will result in two different instances being used as the instantiation
    of the private service is done inline (Örn:  ``new PrivateFooBar()``).

Simply said: A service will be private when you do not want to access it
directly from your code.

Here is an example:

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo
             public: false

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo" public="false" />

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo');
        $definition->setPublic(false);
        $container->setDefinition('foo', $definition);

Now that the service is private, you *cannot* call::

    $container->get('foo');

However, if a service has been marked as private, you can still alias it (see
below) to access this service (via the alias).

.. note::

   Serviss are by default public.

Aliasing
~~~~~~~~

When using core or third party bundles within your application, you may want
to use shortcuts to access some services. You can do so by aliasing them and,
furthermore, you can even alias non-public services.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo
           bar:
             alias: foo

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo"/>

        <service id="bar" alias="foo" />

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo');
        $container->setDefinition('foo', $definition);

        $containerBuilder->setAlias('bar', 'foo');

This means that when using the container directly, you can access the ``foo``
service by asking for the ``bar`` service like this::

    $container->get('bar'); // Would return the foo service

Requiring files
~~~~~~~~~~~~~~~

There might be use cases when you need to include another file just before
the service itself gets loaded. To do so, you can use the ``file`` directive.

.. configuration-block::

    .. code-block:: yaml

        services:
           foo:
             class: Acme\HelloBundle\Foo\Bar
             file: %kernel.root_dir%/src/path/to/file/foo.php

    .. code-block:: xml

        <service id="foo" class="Acme\HelloBundle\Foo\Bar">
            <file>%kernel.root_dir%/src/path/to/file/foo.php</file>
        </service>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Foo\Bar');
        $definition->setFile('%kernel.root_dir%/src/path/to/file/foo.php');
        $container->setDefinition('foo', $definition);

Notice that symfony will internally call the PHP function require_once
which means that your file will be included only once per request.

.. _book-service-container-tags:

Tags (``tags``)
~~~~~~~~~~~~~~~

In the same way that a blog post on the Web might be tagged with things such
as "Symfony" or "PHP", services configured in your container can also be
tagged. In the service container, a tag implies that the service is meant
to be used for a specific purpose. Take the following example:

.. configuration-block::

    .. code-block:: yaml

        services:
            foo.twig.extension:
                class: Acme\HelloBundle\Extension\FooExtension
                tags:
                    -  { name: twig.extension }

    .. code-block:: xml

        <service id="foo.twig.extension" class="Acme\HelloBundle\Extension\FooExtension">
            <tag name="twig.extension" />
        </service>

    .. code-block:: php

        $definition = new Definition('Acme\HelloBundle\Extension\FooExtension');
        $definition->addTag('twig.extension');
        $container->setDefinition('foo.twig.extension', $definition);

The ``twig.extension`` tag is a special tag that the ``TwigBundle`` uses
during configuration. By giving the service this ``twig.extension`` tag,
the bundle knows that the ``foo.twig.extension`` service should be registered
as a Twig extension with Twig. In other words, Twig finds all services tagged
with ``twig.extension`` and automatically registers them as extensions.

Tags, then, are a way to tell Symfony2 or other third-party bundles that
your service should be registered or used in some special way by the bundle.

The following is a list of tags available with the core Symfony2 bundles.
Each of these has a different effect on your service and many tags require
additional arguments (beyond just the ``name`` parameter).

* assetic.filter
* assetic.templating.php
* data_collector
* form.field_factory.guesser
* kernel.cache_warmer
* kernel.event_listener
* monolog.logger
* routing.loader
* security.listener.factory
* security.voter
* templating.helper
* twig.extension
* translation.loader
* validator.constraint_validator

Learn more
----------

* :doc:`/components/dependency_injection/factories`
* :doc:`/components/dependency_injection/parentservices`
* :doc:`/cookbook/controller/service`

.. _`servis tabanlı mimari`: http://wikipedia.org/wiki/Service-oriented_architecture
