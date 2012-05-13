.. index::
   single: Internals (İç Yapı)

Internals (İç Yapı)
===================
Göründüğü kadarıyla Symfony2'nin nasıl çalıştığını ve genişletilebileceğini
anlamak istiyorsunuz. Bu beni çok mutlu ediyor!. Bu ksımda Symfony2'nin
iç yapısına (internals) daha derin bir bakış yapacağız.

.. note::

    Bu bölümü Symfony2'de sahne arkasında neler oluyor ya da Symfony2'yi
    nasıl genişletilebileceğini anlamak ihtiyacı hissediyorsanız okuyun

Genel Bakış
------------

Symfony2 kodu bazı bağımsız katmanlardan oluşur. Her katman bir öncekinin
üzerinde inşa edilmiştir.

.. tip::

    Autoloading framework tarafından direkt kontrol edilmez. Bu bağımsız
    olarak :class:`Symfony\\Component\\ClassLoader\\UniversalClassLoader`
    sınıfı ve  ``src/autoload.php`` dosyası yardımı ile yaplır.
    Daha fazla bilgi için :doc:`dedicated kısmını </components/class_loader>` 
    okuyun.

``HttpFoundation`` Bileşeni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
En alt düzey :namespace:`Symfony\\Component\\HttpFoundation` bileşenidir.
HttpFoundation HTTP işlemlerini halledebilmeniz için ana nesneleri sağlar.
Bu bazı PHP fonksiyonlarının ve değişkenlerinin Nesneye-Yönelimli bir özetidir: 

* :class:`Symfony\\Component\\HttpFoundation\\Request` sınıfı 
  ``$_GET``, ``$_POST``, ``$_COOKIE``, ``$_FILES``, ve ``$_SERVER``
  gibi ana PHP global değişkenlerini özetler;

* :class:`Symfony\\Component\\HttpFoundation\\Response` sınıfı 
  ``header()``, ``setcookie()``, ve ``echo`` gibi bazı PHP fonksiyonlarını
  özetler.

* :class:`Symfony\\Component\\HttpFoundation\\Session` sınıfı ve 
  :class:`Symfony\\Component\\HttpFoundation\\SessionStorage\\SessionStorageInterface`
  interface'i oturum yönetimleri için kullanılan ``session_*()`` functionlarını 7
  özetler.

``HttpKernel`` Bileşeni
~~~~~~~~~~~~~~~~~~~~~~~~
HttpFoundation 'un üzerinde :namespace:`Symfony\\Component\\HttpKernel`
bileşeni bulunur. HttpKernel, HTTP'nin dinamik kısımı olan Request ve Response
sınıflarının üzerinde gelen isteklerin işlenmesi için standart ve basit bir yol
sunar. Aynı zamanda HttpKernel, bir Web Framework'un yeniden yaratılmasında 
çok kafa yorulmaması için araçlar ve uzatma noktalarını da sağlayarak ideal 
bir başlangıç noktasıdır.

İsteğe bağlı olarak Dependency Injection (Bağımlılık enjeksiyonu) bileşeni
ve güçlü eklenti sistemi (bundle) sayesinde de konfügüre edilebilirlik ve
genişletilebilirlik sağlar.

.. seealso::

    Daha fazlası için :doc:`Dependency Injection (Bağımlılık enjeksiyonu) </book/service_container>` 
    ve :doc:`Bundle'lar </cookbook/bundles/best_practices>` kısımlarına bakın.

``FrameworkBundle`` Bundle'ı
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:namespace:`Symfony\\Bundle\\FrameworkBundle` bundle 'ı ana bileşenleri ve 
kütüphaneleri hafif ve hızlı bir MVC frameworku haline getirmek için birleştien
bundle'dır. Ayrıca beraberinde gelen tutarlı bir konfigrüasyon ve kurallar ile
birlikte kolay bir öğrenme eğrisine sahiptir.

.. index::
   single: İç Yapı; Kernel (Çekirdek)

Kernel (Çekirdek)
-----------------
:class:`Symfony\\Component\\HttpKernel\\HttpKernel` sınıfı Symfony2'nin 
merkez sınıfıdır ve istemciden gelen istekleri işlemek ile sorumludur.
Ana amacı :class:`Symfony\\Component\\HttpFoundation\\Request` nesnesinden
:class:`Symfony\\Component\\HttpFoundation\\Response` nesnesine "çevirmektir"


Her Symfony2 Kernel'i 
:class:`Symfony\\Component\\HttpKernel\\HttpKernelInterface` 'inden 
yapılır::

    function handle(Request $request, $type = self::MASTER_REQUEST, $catch = true)

.. index::
   single: İç Yapı; Controller Resolver (Controller Çözümleyicisi)

Controller'lar
~~~~~~~~~~~~~~
Request (istek) 'i Response (Cevap) 'a çevirirken  Çekirdek bir "Controller" 
kullanır. Bir controller geçerli herhangi bir PHP çağırılabileni olabilir.

Kernel, :class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface`
vasıtası ile hangi controller'in çalıştırılacağını belirler::

    public function getController(Request $request);

    public function getArguments(Request $request, $controller);

:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getController`
methodu verilen Request ile bir Controller (bir PHP çağırılanı) 'ı birleştirerek geri döndürme yapar.

Varsayılan olarak (:class:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolver`)
request niteliğine uygun olarak düzenlenmiş bir ``_controller`` aranır.
( "class::method" ``Bundle\BlogBundle\PostController:indexAction`` gibi bir şey olabilir).

.. tip::

    Varsayılan uygulama 
    :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener` sınıfını
    ``_controller`` ın Request niteliğini tanımlamak için kullanır 
    (bkz :ref:`kernel-core-request`).

:method:`Symfony\\Component\\HttpKernel\\Controller\\ControllerResolverInterface::getArguments`
methodu Controller'a gönderilecek olan parametreleri barındıran bir dize değeri döndürür.
Varsayılan uygulama otomatik olarak Request'in niteliğine göre metod argümanlarını çözer.

.. sidebar:: Request niteliklerinden Controller'ın metod argümanlarını eşleştirmek.

    Her metod argümanı için Symfony2 Request niteliğinin değerini aynı isim ile
    almayı dener. Eğer bu tanımlı değilse argüman varsayılan değeri kullanılır. 
    (eğer tanımlı ise)::

        // Symfony2 bir 'id' niteliği (zorunlu)
        // ve bir 'admin' niteliği (opsiyonel) arayacak
        public function showAction($id, $admin = true)
        {
            // ...
        }

.. index::
  single: İç Yapı; Request İşlemek

Request (İstek) 'leri İşlemek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``handle()`` metodu bir ``Request`` değeri alır ve  *daima* bir ``Response``
döndürür.
``handle()``  , ``Request`` 'e çevirmek için Resolver ve bir dizi sıralı
Event notification (Olay Belirteci) 'ne bakar. (Her olay (Event) bilgisi için
bir sonraki bölüme bakınız.):

1. Herhangi bir şey olmadan önce, ``kernel.request`` olayı tetiklenir -- Eğer
   dinleyicilerden (listener) herhangibirisi bir ``Response`` döndürürse
   8. adıma direkt olarak atlanır;

2. Resolver çalıştırılacak Controller'in belirlenmesi için çağırılır;


3. ``kernel.controller`` event'ının dinleyicileri şu anda Controller'ı 
   istedikleri gibi değiştirebilime yeteneğine sahiptir. (değiştir, paketle, ...);
   
4. Kernel Controller'ın gerçekten bir PHP cağırılabiliri olduğunu kontrol eder;

5. Resolver Controller'a gönderilecek argümanları belirlemek üzere çağırılır;

6. Kernel, Controller'ı çağırır;

7. Eğer Controller bir ``Response`` döndürmüyorsa, ``kernel.view`` olayı
   dinleyicileri (listeners) Controller'i  çevirdiği değeri ``Response``
   olarak çevirebilirler; 

8. ``kernel.response`` olayı dinşeyicileri ``Response`` 'u değişitrebilirler
   (içerik ve başlıklarını)

9. Response çevrilir.

Eğer işlem sırasında bir Exception(İstisna) meydana gelirse ``kernel.exception``
tetiklenir ve dinleyiciler (listener) İstisnayı bir Response'a çevirmek için bir
şans verirler. Eğer çalışırsa ``kernel.response`` tetiklenir; çalışmazsa 
İstisna yeniden atılır.

Eğer istisnaların yakalanmasını istemiyorsanuz (olay içerisinde gömülü istekler için)
``kernel.exception`` kapatmak için ``handle()`` metodunun üçüncü parametresini 
``false`` olarak değiştirin.

.. index::
  single: İç Yapı; İçsel İstekler (Requests)

İçsel İstekler (Requests)
~~~~~~~~~~~~~~~~~~~~~~~~~
Request'in işlendiği ('ana' olanı) herhangi bir zamanda bir alt-istek de 
işlenebilir. ``handle()`` metoduna bir istek tipi gönderebilirsiniz (ikinci argümanına):


* ``HttpKernelInterface::MASTER_REQUEST``;
* ``HttpKernelInterface::SUB_REQUEST``.

Gönderilen bu tipe göre tüm event (olay) ve listener (dinleyiciler) hareket
edebilirler. (bazı süreçler sadece ana istekten önce oluşmalıdır)

.. index::
   pair: Kernel; Event (olay)

Olaylar (Events)
~~~~~~~~~~~~~~~~

Her olay Kernel'in bir alt sınıfı olan 
:class:`Symfony\\Component\\HttpKernel\\Event\\KernelEvent` sınıfıdan türer.
Bunun anlamı her olay şu aynı temel bilgilere ulaşır demektir:

* ``getRequestType()`` - isteğin *tipini* döndürür
  (``HttpKernelInterface::MASTER_REQUEST`` ya da ``HttpKernelInterface::SUB_REQUEST``);

* ``getKernel()`` - isteği işleyen Kernel'i döndürür;

* ``getRequest()`` - düzenlenecek olan geçerli ``Request`` 'i döndürür.

``getRequestType()``
....................

``getRequestType()`` methodu listenere isteğin tipini öğrenmesini sağlar.
Örneğin eğer bir listener sadece ana istekler için akif olmalı ise listener
metodunuzun başına şu kodu eklemelisiniz::

    use Symfony\Component\HttpKernel\HttpKernelInterface;

    if (HttpKernelInterface::MASTER_REQUEST !== $event->getRequestType()) {
        // return immediately
        return;
    }

.. tip::

    Eğer henüz Symfony2 Event Dispatcher ile tanışmadıysanız, önce
    :doc:`Event Dispatcher Bileşen Belgesi</components/event_dispatcher/introduction>`
    kısmını okuyun.

.. index::
   single: Event; kernel.request

.. _kernel-core-request:

``kernel.request`` Event'i (olayı)
..................................

*Event Sınıfı*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseEvent`

Bu event'in amacı acilen bir ``Response`` objesi döndürmek ya da Controller'ın
eventtan sonra çağırılabilmesi için gerekli değişkenleri set etmektir.
Herhangi bir listener ``Response`` nesnesini event içerisindeki ``setResponse()`` 
metodu aracılığı ile çevirebilir. Bu durumda diğer tüm listener'lar çağırılamayacaktır.

Bu event ``FrameworkBundle`` bundle'ı kullanarak  ``_controller`` ``Request``
niteliklerini :class:`Symfony\\Bundle\\FrameworkBundle\\EventListener\\RouterListener` 
sınıfı aracılığı ile toplar.
RequestListener :class:`Symfony\\Component\\Routing\\RouterInterface` nesnesini
``Request` 'i eşlemek ve Controller adını belirlemek için kullanır. 
(``_controller`` 'ın ``Request`` niteliğinde saklanan değer üzerinden)


.. index::
   single: Event; kernel.controller

``kernel.controller`` Event'i (olayı)
.....................................

*Event Sınıfı*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterControllerEvent`

Bu event ``FrameworkBundle`` tarafından kullanılmaz ancak çalıştrıacak olan
controller'i değiştirmek için kullanılacak ana noktadır:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\FilterControllerEvent;

    public function onKernelController(FilterControllerEvent $event)
    {
        $controller = $event->getController();
        // ...

        // Controller herhangi bir PHP çağırılabileni tarafından
        // değiştirilebilir.
        $event->setController($controller);
    }

.. index::
   single: Event; kernel.view

``kernel.view`` Event'i (olayı)
.....................

*Event Sınıfı*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForControllerResultEvent`

Bu event ``FrameworkBundle`` tarafından kullanılmaz ancak görünümü düzenlemek
için kullanılabilir. Bu event *sadece* eğer controller bir ``Response`` nesnesi
*döndürmezse* kullanılır. Bu event'in amacı ``Response`` nesnesine çevrilebilecek diğer
şeylere izin vermektir.

Controller'in çevirdiği değere ``getControllerResult`` metodu
ile erişilebilir:: 

    use Symfony\Component\HttpKernel\Event\GetResponseForControllerResultEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelView(GetResponseForControllerResultEvent $event)
    {
        $val = $event->getControllerResult();
        $response = new Response();
        // Response değerinin nasıl değişitirlebileceği
        // hakkındaki bazı kodlar...

        $event->setResponse($response);
    }

.. index::
   single: Event; kernel.response

``kernel.response`` Event'i (olayı)
...................................

*Event Sınıfı*: :class:`Symfony\\Component\\HttpKernel\\Event\\FilterResponseEvent`

Bu eventin amacı diğer sistemlerin ``Response`` nesnesini yaratıldıktan sonra
değiştirebilmelerine olanak vermektir:

.. code-block:: php

    public function onKernelResponse(FilterResponseEvent $event)
    {
        $response = $event->getResponse();
        // .. response nesnesini değiştir...
    }

``FrameworkBundle`` bazı listenerleri kayıt eder:

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ProfilerListener`:
  geçerli requestteki verileri toplar;

* :class:`Symfony\\Bundle\\WebProfilerBundle\\EventListener\\WebDebugToolbarListener`:
  Web Debug Araç Çubuğunu ekler;

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\ResponseListener`: 
  Response'un ``Content-Type`` 'ını gelen isteğe göre düzeltir.

* :class:`Symfony\\Component\\HttpKernel\\EventListener\\EsiListener`: adds a
  ``Surrogate-Control`` HTTP header when the Response needs to be parsed for
  ESI tags.

.. index::
   single: Event; kernel.exception

.. _kernel-kernel.exception:

``kernel.exception`` Event'i (olayı)
....................................

*Event Sınıfı*: :class:`Symfony\\Component\\HttpKernel\\Event\\GetResponseForExceptionEvent`

``FrameworkBundle`` 'ı 
:class:`Symfony\\Component\\HttpKernel\\EventListener\\ExceptionListener`
sınıfını ``Request`` 'i verilen Controller'a göndermesi için kayıt eder
(``exception_listener.controller`` parametresinin değeri --
``sinif::metod`` yazım şeklinde olmalıdır)


Bu event üzerindeki bir listener bir ``Response`` nesnesi yaratabilir set
edebilir  yeni bir ``Exception`` nesnesi yaratabilir, set edebilir ya da 
hiç bir şey yapmaz:

.. code-block:: php

    use Symfony\Component\HttpKernel\Event\GetResponseForExceptionEvent;
    use Symfony\Component\HttpFoundation\Response;

    public function onKernelException(GetResponseForExceptionEvent $event)
    {
        $exception = $event->getException();
        $response = new Response();
        // Yakalanan İstisnaya göre bir Response nesnesi yarat
        $event->setResponse($response);

        // Alternatif olarak bir İstisna yaratabilirsiniz
        // $exception = new \Exception('Özel bir İstisna');
        // $event->setException($exception);
    }

.. index::
   single: Event Dispatcher

Event Dispatcher
--------------------

Event dispatcher bir Symfony isteğinin (request) altındaki pek çok mantıksal işlemleri ve 
akışlardan sorumlu olan kendi başına çalışabilen bir bileşendir. 
Daha fazla bilgi için :doc:`Event Dispatcher Bileşeni Belgesi </components/event_dispatcher/introduction>`
'ni okuyun.

.. index::
   single: Profiler

.. _internals-profiler:

Profiler
--------
Aktif edildiğinde, Symfony2 Profiller, yapılan her istek (request) için 
daha soradan saklayıp analiz edebilmeniz için pek çok kullanışlı bilgi toplar.
Profilleri geliştirme ortamında kullanmanı durumunda bu araç size
kodu inceleme ve performansı geliştirme açısından yardımcı olabilir. Production
(Ürün) tarafında kullandığınızda sorunlar ortaya çıktıktan sonra neden çıktığı
konusunda size bir fikir verebilir.

Symfony2'nin sizlere sunmuş olduğu Web Debug Toolbar ve Web Profiller adındaki
görsel veri araçlarını direkt olarak nadiren kullanmışsınızdır. Eğer Symfony2 Standart
Edition kullanıyorsanız profiler, web debug toolbar ve web profiler araçlarının 
tamamı sizin için düzgün bir şekilde ayarlanmış ve kullanımınıza hazırlanmıştır.

.. note::

    Profiler füm request'ler (istekler) (basit istekler, yönlendirmeler, 
    istisnalar, ajax istekleri, ESI istekleri gibi pek çok form ve şekildeki 
    HTTP metodlarını) için bilgi toplar.
    Bunun anlamı tel bir URL için birden fazla farklı profiling verisi
    elde edebilirsiniz.

.. index::
   single: Profiler; Görselleştirme

Profiling Verisini Görselleştirme
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Web Debug Toolbar'ı kullanmak
..............................

Geliştirme ortamında web debug toolbar'ı sayfanın altında bulunur. Bu
kısım istediğiniz zaman kapayabileceğiniz istediğiniz zaman erişebileceğiniz
şekilde profile verisi hakkında güzel bilgiler içerir

Eğer Web Debug Toolbar üzerinde sağlanan veri yetersiz gelirse token linkine
(13 hanelirastgele karakterlerden oluşan bir metin) tıklayarak Web Profiler
aracına ulaşabilirsiniz.

.. note::

    Eğer token tıklanamaz bir durumda ise bunun anlamı profiler isteği
    tam olarak anlamamış demektir. (bunun için altındaki konfigürasyon bilgisi
    kısmına bakın) 

Web Profiler aracıyla Profile verisini Analiz Etmek
....................................................

Web profiler geliştirme ortamında kodunuzu debug edebilmeniz ve performansı
iyileştirmeniz için ayrıca da production ortamında ortaya çıkan sorunları
belirleyebilmeniz için verilerinizi görselleştiren bir araçtır.
Profiller'ın topladığı tüm veri web arabirimi üzerinden görülebilmektedir.

.. index::
   single: Profiler; Profiler servisini kullanmak

Profiling Bilgisine Ulaşmak
...........................

Profile verisini görselleştirmek için başka bir görselleştirme aracı
kullanmanıza gerek yok. Fakat özel bir isteğin gerçekleştikten sonraki
profile bilgilerini nasıl bulacaksınız?. Profiler bir istek hakkında verileri
saklarken aynı zamanda onu ilgili token'ı ile de ilişkilendirerek saklar. Bu
token değeri Response içerisindeki ``X-Debug-Token`` adlı HTTP başlığında saklanır::


    $profile = $container->get('profiler')->loadProfileFromResponse($response);

    $profile = $container->get('profiler')->loadProfile($token);

.. tip::

    Profiler açık ancak web debug toolbar'ında gözükmüyorsa ya da Token
    değerini bir Ajax isteği ise bu durumda Firebuf gibi bir araç 
    kullanarak HTTP başlığında gelen ``X-Debug-Token`` değerini öğrenebilirsiniz.

``find()`` metod'unu kullanarak bazı filitreleme özellikleri kullanarak 
token'lara ulaşabilirsiniz::

    // Son 10 token'ı al
    $tokens = $container->get('profiler')->find('', '', 10);

    // URL'de /admin/ içeren 10 token'ı al
    $tokens = $container->get('profiler')->find('', '/admin/', 10);

    // lokal istekleri içeren Son 10 token'ı al
    $tokens = $container->get('profiler')->find('127.0.0.1', '', 10);

Eğer profiling verisini başka bir makinede değiştirmek istiyorsanız bu durumda
yaratılan veriyi almak ya da vermek için ``export()`` ve ``import()`` metodları
kullanılır::

    // on the production machine
    $profile = $container->get('profiler')->loadProfile($token);
    $data = $profiler->export($profile);

    // on the development machine
    $profiler->import($data);

.. index::
   single: Profiler; Görselleştirme

Konfigürasyon
.............

Varsayılan Symfony2 konfigürasyonu profiler, web debug toolbar ve web 
profiler araçlarının kullanması için ayarlı bir şekilde gelir. Burada
geliştirme ortamına ait olan konfigürasyon dosyasının içini görmektesiniz::

.. configuration-block::

    .. code-block:: yaml

        # profiler'ı yükle
        framework:
            profiler: { only_exceptions: false }

        # web profiler'ı aç
        web_profiler:
            toolbar: true
            intercept_redirects: true
            verbose: true

    .. code-block:: xml

        <!-- xmlns:webprofiler="http://symfony.com/schema/dic/webprofiler" -->
        <!-- xsi:schemaLocation="http://symfony.com/schema/dic/webprofiler http://symfony.com/schema/dic/webprofiler/webprofiler-1.0.xsd"> -->

        <!-- profiler'ı yükle -->
        <framework:config>
            <framework:profiler only-exceptions="false" />
        </framework:config>

        <!-- web profiler'ı aç -->
        <webprofiler:config
            toolbar="true"
            intercept-redirects="true"
            verbose="true"
        />

    .. code-block:: php

        // profiler'ı yükle
        $container->loadFromExtension('framework', array(
            'profiler' => array('only-exceptions' => false),
        ));

        // web profiler'ı aç
        $container->loadFromExtension('web_profiler', array(
            'toolbar' => true,
            'intercept-redirects' => true,
            'verbose' => true,
        ));

``only-exceptions`` parametresi ``true`` yapıldığında profiler uygulamada
bir exception (istisna) oluştuğunda veri toplayacaktır.

``intercept-redirects`` parametresi ``true`` yapıldığında profiler 
yönlendirmelerin (redirect) 'ların tamamını yakalayacak ve redirect gerçekleşmeden
önceki veriyi incelemek için size bir fırsat verecektir.

``verbose`` parametresi ``true`` yapıldığında Web Debug Toolbar üzerinde 
çok fazla veri gözükecektir. ``verbose`` parametresi ``false`` yapıldığında ise
ikincil veriler saklanacak ve toolbar daha kısa olacaktır. 

Eğer web profiler'ı aktif ederseniz aynı zamanda profiler yönlendirmelerinide (route)
açmanız gerekmektedir::

.. configuration-block::

    .. code-block:: yaml

        _profiler:
            resource: @WebProfilerBundle/Resources/config/routing/profiler.xml
            prefix:   /_profiler

    .. code-block:: xml

        <import resource="@WebProfilerBundle/Resources/config/routing/profiler.xml" prefix="/_profiler" />

    .. code-block:: php

        $collection->addCollection($loader->import("@WebProfilerBundle/Resources/config/routing/profiler.xml"), '/_profiler');


Eğer profiler çok fazla şey açtıysa production ortamında belki sadece 
belirli durumlar altında aktif etmek isteyebilirsiniz.``only-exceptions`` 
ayarı 500 sayfalarında profillemeyi limitler ancak eğer belirlediğiniz başka
ip'lerden gelen bilgileri ya da web sitesinin sadece belirli kısımlarındaki
bilgileri almak isterseniz? Bu durumda istek eşleştiricisini (request matcher)
kullanabiirsiniz::

.. configuration-block::

    .. code-block:: yaml

        # profiler'ı sadece 192.168.0.0 ağından istek geldiğinde aktif et.
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24 }

        # profiler sadece /admin URL'lerinde aktif olur.
        framework:
            profiler:
                matcher: { path: "^/admin/" }

        # kuralların kombinasyonu
        framework:
            profiler:
                matcher: { ip: 192.168.0.0/24, path: "^/admin/" }

        # "custom_matcher" olarak tanımlanan özel bir eşleştirici hizmeti kullanmak için
        framework:
            profiler:
                matcher: { service: custom_matcher }

    .. code-block:: xml

        <!-- profiler'ı sadece 192.168.0.0 ağından istek geldiğinde aktif et. -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" />
            </framework:profiler>
        </framework:config>

        <!-- profiler sadece /admin URL'lerinde aktif olur.-->
        <framework:config>
            <framework:profiler>
                <framework:matcher path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- kuralların kombinasyonu -->
        <framework:config>
            <framework:profiler>
                <framework:matcher ip="192.168.0.0/24" path="^/admin/" />
            </framework:profiler>
        </framework:config>

        <!-- "custom_matcher" olarak tanımlanan özel bir eşleştirici hizmeti kullanmak için -->
        <framework:config>
            <framework:profiler>
                <framework:matcher service="custom_matcher" />
            </framework:profiler>
        </framework:config>

    .. code-block:: php

        // profiler'ı sadece 192.168.0.0 ağından istek geldiğinde aktif et.
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24'),
            ),
        ));

        // profiler sadece /admin URL'lerinde aktif olur.
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('path' => '^/admin/'),
            ),
        ));

        // kuralların kombinasyonu
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('ip' => '192.168.0.0/24', 'path' => '^/admin/'),
            ),
        ));

        # "custom_matcher" olarak tanımlanan özel bir eşleştirici hizmeti kullanmak için
        $container->loadFromExtension('framework', array(
            'profiler' => array(
                'matcher' => array('service' => 'custom_matcher'),
            ),
        ));

Tarif Kitabından Daha Fazlasını Öğrenin
----------------------------------------

* :doc:`/cookbook/testing/profiling`
* :doc:`/cookbook/profiler/data_collector`
* :doc:`/cookbook/event_dispatcher/class_extension`
* :doc:`/cookbook/event_dispatcher/method_behavior`

.. _`Symfony2 Dependency Injection component`: https://github.com/symfony/DependencyInjection
