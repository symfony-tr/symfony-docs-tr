.. index::
   single: Controller

Controller
==========

Bir controller, yaratmış olduğunuz HTTP isteğinden bilgiyi alarak bir HTTP
cevabı (Symfony2 'de ``Response`` nesnesi gibi) döndüren PHP fonksiyonudur.
Cevap, bir HTML sayfası , bir XML dökümanı, serileştirilmiş bir JSON dizesi (array),
bir resim, bir yönlendirme (redirect), bir 404 hatası ya da düşünebildiğiniz
herhangi bir şey olabilir. Controller *uygulamanızın*  sayfa içeriğini oluşturmadaki
herhangi bir şeyi kapsayabilir. 

Bunun nasıl basit bir şey olduğunu görmek için Symfony2 Controller'ini
uygulamada görelim. Aşağıdaki controller ekrana basitçe ``Hello world!`` 
yazacaktır::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

Bir controller'in amacı her zaman aynıdır. Bir ``Response`` objesi döndürmek.
Bunu yaparken istekten bazı bilgileri okuyabilir, bir veritabanı kaynağını
çağırabilir, e-posta gönderebilir ya da kullanıcının oturumuna herhangi
bir bilgi yazabilir. Fakat bütün bu durumlarda, controller, sonuçta istemciye
geri gönderilmek üzere bir ``Response`` nesnesi döndürecektir.

Herhangi bir sihir yok ve diğer gereklilikler için endişelenmeyin! Burada
bir kaç yaygın örnek var:

* *Controller A* sitenin ana sayfasını temsil eden içeriği ``Response`` 
  nesnesi olarak hazırlar.

* *Controller B* Bu blog için veritabanından okunup gelecek değeri ekranda
  gösterebilmek için bir ``Response`` objesi yaratmak amacıyla , ``slug`` adındaki
  parametreyi okur. Eğer ``slug`` veritabanında bulunamazsa 404 durum 
  koduyla birlikte bir ``Response`` nesnesi yaratır.
  
* *Controller C* iletişim formunu işler. İstek (request) üzerinden gelen
  bilgileri okur ve iletişim bilgilerini webmaster'a göndermek için veritabanına 
  yazar. En sonunda bir ``Response`` nesnesi yaratarak istemcinin tarayıcısına
  iletişim sayfasında bilgilerin alındığını belirten "teşekkürler" sayfasını
  gönderir.

.. index::
   single: Controller; Request(istek)-controller-response(cevap) döngüsü

Requests, Controller, Response döngüsü
----------------------------------------
Her istek (request) Symfony2 projesinde basit bir döngü ile işlenir.
Framework tekrarlayan süreçlere dikkat ederek en sonunda özelleştirilmiş uygulama
kodunun evsahipliği yaptığı bir controller çalıştırır:

#. Her istek uygulamanın başlatılmasını sağlayan bir front controller 
   tarafından işlenir.(Örn. ``app.php`` ya da ``app_dev.php``)

#. ``Router`` istekten gelen bilgiyi okur, Bu bilgi ile (Örn. URI) 
   route üzerinden gelen ``_controller`` parametresinde eşleşen route
   bilgisini bulur.

#. Eşleşen route üzerinde belirtilen controller çalıştırılır ve controller
   içerisindeki kod bir ``Response`` nesnesi yaratarak geri döndürür;

#. HTTP başlıkları ve ``Response`` nesnesinin içeriği istemciye geri döndürülür.

Sayfa yaratmak controller yaratmak kadar basittir (#3) ve yapılan route URL'yi
controller ile eşleştirir (#2).

.. note::

    "front controller" isim olarak benzemesine rağmen biz bu bölümde
    "controller" 'lardan bahsedeceğiz.Bir front controller bütün istekleri
    yöneten, web klasöründe olan, kısa bir PHP dosyasıdır. Tipik bir uygulama
    bir production (ürün) front controllerine (örn: ``app.php``) ve 
    development(geliştirme) front controllerine (örn: ``app_dev.php``)
    sahip olacaktır. Muhtemelen bu dosyaları asla düzenlemeyeceğiniz 
    için uygulamanızdaki front controller'lar için endişelenmenize
    gerek yok.

.. index::
   single: Controller; Basit Örnek

Basit bir Controller
-------------------
Symfony2 'de herhangi bir PHP cağırılabileni (callable) (bir fonksiyon,
bir nesnenin metodu ya da ``Closure``) bir controller olabilir iken, genellikle
controller nesnesinin içinde bir metod bulunur. Controller'lar aynı şekilde 
*aksiyonlar* (actions) olarak da bilinirler.


.. code-block:: php
    :linenos:

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

.. tip::

    *controller* ın , *controller sınıfı* (``HelloController``) içindeki
    ``indexAction`` metodu olduğuna dikkat edin. *controller sınıfı* terimi
    kafanızı karıştırmasın. Bu tip bir uygulama sadece çeşitli controller/aksiyon
    'ları bir arada tutmak için kullanılır. Tipik olarak controller sınıfı pek çok
    controller/aksiyona ev sahipliği yapar (Örn. ``updateAction``, ``deleteAction``,
    vs).

Bu controller olukça açık ancak yine de açıklayalım:

* *satır 3*: Symfony2 geçerli controller'in namespace'i için 
  PHP 5.3 namespace özelliğinin avantajlarını kullanır. ``use`` anahtar kelimesi
  controllerimizin geri döndürmesi gereken ``Response`` sınıfını içeri aktarır.

* *satır 6*: Sınıf ismi controller sınıfının kısaltılmış hali (örn. ``Hello``) 
  ve ``Controller`` teriminin birleşiminden oluşur. Bu kullanım controller'ların
  tutarlı olmasını routing konfigürasyonunda adlarının sadece ilk kısımlarının
  (örn. ``Hello``) kullanılabilmesine olanak sağlar. 

* *satır 8*: Controller sınıfının içindeki her aksiyon ``Action`` son eki
  ile ifade edilir ve routing konfigürasyonunda aksiyonun ismi ile (``index``) gösterilir.
  Sonraki kısımda bu aksiyon için bir URI ile eşleşen route yaratacaksınız.
  Route yer tutucularının  (``{name}``) aksiyon metodlarının argümanlarına 
  (``$name``) nasıl döndüğünü göreceksiniz.
  
* *satır 10*: Controller bir ``Response`` nesnesi yaratır ve döndürür.

.. index::
   single: Controller; Route'lar ve controller'lar

Controller için bir URI Eşleştirmek
------------------------------------
Yeni controller basit bir HTML sayfası döndürmektedir. Gerçekte bu sayfayı görebilmeniz
için özel bir URL deseni olan bir route ile controller'ı eşleştirmeniz gerekir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

Şimdi ``/hello/ryan`` olduğunda ``HelloController::indexAction()`` 
controlleri çalıştırılacak ve ``ryan`` değeri``$name`` değişkenine
gönderilecek. "Sayfa" yaratmanın anlamı basitçe bir controller metodu
yaratmak ve bunu bir route ile birleştirmektir.

Controller'i ifade eden ``AcmeHelloBundle:Hello:index`` yazımına dikkat edin.
Symfony2 farklı controllerları ifade edebilmek için esnek bir yazım sistemi
kullanır. Bu sık kullanılan yazım şekli Symfony2'ye ``AcmeHelloBundle`` olarak
adlandırılan bir bundle içerisindeki ``HelloController`` sınıfını çalıştırmasını
söyler. ``indexAction()`` metodu daha sonra çalıştırılır.

Farklı controller'lar için yazım şekli hakkında daha fazla bilgi almak için
:ref:`controller-string-syntax` belgesine bakın.

.. note::

    Bu örnekler için routing (yönlendirme) konfigürasyonlarının yeri, direkt olarak
    ``app/config/`` klasörüdür. En iyi yöntem route'lar hangi bundle'ı işaret ediyorsa
    o bundle içerisinde bu konfigürasyonu yapmaktır. Bu konuda daha fazla  bilgi için
    :ref:`routing-include-external-resources` belgesine bakın.

.. tip::

    Routing (Yönlendirme) sistemi hakkında daha fazla bilgiyi 
    :doc:`Routing (Yönlendirme) kısmından</book/routing>` öğrenebilirsiniz.

.. index::
   single: Controller; Controller argümanları

.. _route-parameters-controller-arguments:

Controller Argümanları için Route Parametreleri
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Artık ``_controller`` parametresindeki ``AcmeHelloBundle:Hello:index``
ifadesinin ``AcmeHelloBundle`` bundle içerisinde bulunan 
``HelloController::indexAction()`` metoduna işaret ettiğini biliyorsunuz.
Dahada ilginç olanı argümanlar bu metoda aktarıldı:

.. code-block:: php

    <?php
    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          // ...
        }
    }

Controller'ın ``$name`` adında, eşleşen route için ``{name}``  parametresi
ile ilişkili, (bizim örneğimizde ``ryan``) tek bir argümanı var. Aslında
controller'iniz çalıştırıldığında, Symfony2 eşleşen yönlendirmedeki 
parametreleri ilgili controller'ın argümanları ile eşleştirir. Şu örneğe
bakalım:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        hello:
            pattern:      /hello/{first_name}/{last_name}
            defaults:     { _controller: AcmeHelloBundle:Hello:index, color: green }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <route id="hello" pattern="/hello/{first_name}/{last_name}">
            <default key="_controller">AcmeHelloBundle:Hello:index</default>
            <default key="color">green</default>
        </route>

    .. code-block:: php

        // app/config/routing.php
        $collection->add('hello', new Route('/hello/{first_name}/{last_name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
            'color'       => 'green',
        )));

Controller bazı argümanlar alabilir::

    public function indexAction($first_name, $last_name, $color)
    {
        // ...
    }

İki yer tutucu değişkeninin (``{first_name}``, ``{last_name}``) ve varsayılan
değeri, atanmış olan ``color`` değişkeninin controller'in argümanları olduğuna
dikkat edin. Route eşleştiği zaman placeholder değişkenleri ``defaults`` ile
birleştirilir ve controllerda olan değişkenler için bir dize değişkenine çevrilir.

Route parametrelerini controller argümanları ile eşleştirmek kolay ve esnektir.
Sadece geliştirme süreci içerisinde şu kuralları aklınızıda tutun.

* **Controller'daki argümanların sırası önemli değidir.**

    Symfony route içerisindeki parametre isimleri ile controller'ın metodlarındaki
    argümanların adlarını eşleştirebilir. Diğer bir ifade ile ``{last_name}`` 
    parametresi ``$last_name`` argümanı ile eşleştirilir.
    Controller'ın tüm argümanları yeniden sıralansa bile bu durum mükemmel
    çalışır::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Gerekli olan her controller argümanı bir route parametresi ile eşleşmelidir.**

    Aşağıdaki kod bir ``RuntimeException`` istisnası yaratacaktır. Çünkü ``foo`` 
    parametresi route içerisinde tanımlanmadı::

        public function indexAction($first_name, $last_name, $color, $foo)
        {
            // ..
        }

    Argümanları isteğe göre yapmakta mümkündür. Aşağıdaki örnek bir istisna
    (exception) atmayacaktır::

        public function indexAction($first_name, $last_name, $color, $foo = 'bar')
        {
            // ..
        }

* **Tüm routing parametreleri controllerınızda bir argüman olmak zorunda değildir.**

    Eğer, örneğin ``last_name`` controller'ınız için çok önemli değil ise, onu
    tamamen atlayabilirsiniz::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Her route (yönlendirme) özel bir ``_route`` parametresine sahiptir.Bu
    parametre, eşleşen route 'un ismini tutar(örn: ``hello``). Bu 
    çok kullanışlı olmamasına rağmen bu bir controller argümanı
    olarak da kullanılabilir.

.. _book-controller-request-argument:

Controller Argümanı olarak ``Request``
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Kolaylık olması için ayrıca Symfonyde controller'ınıza argüman olarak
``Request`` nesnesini gönderebilirsiniz. Bu özellikle form'larla çalışırken
büyük kolaylık sağlar. Örneğin::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Controller; Temel controller sınıfı

Temel Controller Sınıfı
-------------------------

Kolaylık olması açısından Symfony2, bazı genel controller görevlerine yardım etmek
için ve controller sınıfına gerektiğinde herhangi bir kaynaktan erişmek için
bir temel ``Controller`` sınıfı bile birlikte gelir. Sınıfınızı bu ``Controller``
sınıfı ile genişlettiğinizde bazı yararlı yardımcı metodlara erişebilirsiniz.

``use`` ifadesi ile bu ``Controller`` sınıfını çağırın ve ``HelloController`` 
sınıfını şu şekilde değiştirerek extend edin (genişletin):

.. code-block:: php

    // src/Acme/HelloBundle/Controller/HelloController.php

    namespace Acme\HelloBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\HttpFoundation\Response;

    class HelloController extends Controller
    {
        public function indexAction($name)
        {
          return new Response('<html><body>Hello '.$name.'!</body></html>');
        }
    }

Bu gerçekte controllerinizin çalışmasına herhangi bir etkide bulumaz.
Sonraki bölümde temel controller sınıfının yardımcı metodlarını öğreneceksiniz.
Bu metodlar temel ``Controller`` sınıfıyla gelen ya da gelmeyen çekirdek Symfony2 özelliklerinin
kısa yollarını kullanmanızı sağlar. En iyi yol, bu çekirdek özelliklerinin
nasıl çalıştığını uygulamada :class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` 
sınıfının kendi içerisinde görmektir.

.. tip::

    Symfony de temel sınıftan türetme yapmak *isteğe bağlıdır*. Bu bazı 
    kullanışlı fonksiyonlar sağlar ancak zorunlu değildir. Ayrıca 
    ``Symfony\Component\DependencyInjection\ContainerAware`` sınıfından da
    sınıfı genişletebilirsiniz. Servis taşıyıcı (Service container)
    nesnesi ``container`` değişkeni ile bu işlemden sonra erişilebilir
    hale gelir.

.. note::

    Ayrıca kendi :doc:`Controller'larınızı Servis gibi</cookbook/controller/service>`
    tanımlayabilirsiniz. 

.. index::
   single: Controller; Genel İşlemler

Genel Controller İşlemleri
--------------------------

Controller sanal olarak herşeyi yapabilmesine rağmen çoğu controllerda
temel bazı işlemler tekrar tekrar gerçekleştirecektir. Bu işlemler,
redirect (yönlendirme), forwarding(iletme), şablonları ekrana basma ve
çekirdek hizmetlere erişmek gibi Symfony2 'de kolaylıkla yönetilen işlemlerdir.

.. index::
   single: Controller; Redirecting(Yönlendirme)

Redirecting(Yönlendirme)
~~~~~~~~~~~~~~~~~~~~~~~~

Eğer kullanıcıyı başka bir sayfaya yönlendirmek istiyorsanız ``redirect()`` metodunu
kullanırsınız::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

``generateUrl()`` metodu sadece verilen yol(route) için URL yaratan bir yardımcı
metoddur.Daha fazla bilgi için :doc:`Routing (Yönlendirme)</book/routing>`
bölümüne bakın.

Varsayılan olarak ``redirect()`` metodu bir 302 (geçici) yönlendirme gerçekleştirir.
Bunu 301 (kalıcı) yönlendirmesi yapmak için ikinci argümanı değiştirmelisiniz::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    ``redirect()`` metodu basitçe kullanıcıyı yönlendiren bir ``Response``
    nesnesi yaratır. Bu şuna eşittir:

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controller; Forwarding (iletme)

Forwarding (iletme)
~~~~~~~~~~~~~~~~~~~

Ayrıca kolaylıkla başka bir controller'a da içsel olarak ``forward()`` metodu
ile kolaylıkla iletme yapabilirsiniz. Kullanıcının tarayıcısından yönlendime 
yapmak yerine bu işlem, bir alt istek açar ve ilgili controller'i çağırır. 
``forward()`` metodu iletim yapılan ilgili controller'dan bir ``Response``
nesnesi döndürür::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // response'u değiştirmeye devam et ya da direkt olarak döndür
                
        return $response;
    }

`forward()` metodunun controller'in routing konfigürasyonunda controller'i
temsil eden aynı string içerisinde kullanıldığına dikkat edin. Bu durumda
hedef olan controller sınıfı ``AcmeHelloBundle`` içerisindeki 
``HelloController`` olacaktır. Controller'da sonuçlandırılması için gereken
parametreler array (dize) halinde gönderilecektir. Bu yöntem aynı
controller'lardan şablonlara veri aktarılması için kullanılan bir yöntemdir.
(bkz. :ref:`templating-embedding-controller`) Hedef controller metodu aşağıdaki
şekilde olmalıdır::

    public function fancyAction($name, $color)
    {
        // ... bir Response nesnesi yarat ve döndür.
    }

Yine route için bir controller yaratımındaki gibi argümanların ``fancyAction``
metoduna gönderilmesine sıralı olması bir anlam ifade etmez. Symfony2 index
anahtar isimleri (örn. ``name``) ile metod argüman isimlerini (örn. ``$name``) eşler.
Eğer argüman sıralamasını değiştiriseniz Symfony2 yine her değişken için
doğru değeri iletmeye devam edecektir.

.. tip::

    Diğer temel ``Controller`` metodları gibi ``forward`` metodu sadece
    Symfony2 özelliklerinin kısa yollarından birisidir. Aslında iletim direkt
    olarak ``http_kernel`` hizmeti sayesinde olur. İletim bir ``Response``
    nesnesi çevirir::
    
        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: Controller; Şablonları Ekrana Basmak.

.. _controller-rendering-templates:

Şablonları Ekrana Basmak
~~~~~~~~~~~~~~~~~~~~~~~~

Çok gerekmemesine rağmen, çoğu controller eninde sonunda controller için
HTML yaratmaktan sorumlu ( ya da diğer bir formatta) bir şablonu ekrana basacaktır.
``renderView()`` metodu şablonu ekrana basar ve şablon içindeki içeriği döndürür.
Şablondan gelen içerik bir ``Response`` nesnesi yaratmak için kullanılır::

    $content = $this->renderView('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

    return new Response($content);

Bu işlem aynı zamanda ``render()`` metodunu kullanarak tek adımda da 
yapılabilir::

    return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

İki durumda da ``AcmeHelloBundle`` içerisindeki ``Resources/views/Hello/index.html.twig``
şablonu ekrana basılacaktır.

Symfony şablon motorunun en muhteşem ayrıntıları ile :doc:`Şablonlama(Templating) </book/templating>` kısmında
açıklanmıştır.

.. tip::

    ``renderView`` metodu ``templating`` servisini direkt kullanılması için
    bir kısa yoldur. ``templating`` servisi istenirse direkt olarak da 
    kullanılabilir::
    
        $templating = $this->get('templating');
        $content = $templating->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

.. index::
   single: Controller; Hizmetlere erişmek

Diğer Hizmetlere Erişmek
~~~~~~~~~~~~~~~~~~~~~~~~

Controller sınıfını temel controller sınıfı ile genişlettiğinizde herhangi
Symfony2 servisine ``get()`` metodu ile erişebilirsiniz. Aşağıda ihtiyacınız
olabilecek bazı genel servislere erişme yolu gösterilmiştir:: 

    $request = $this->getRequest();

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

Sayısız servis bulunmaktadır ve sizde eğer isterseniz kendi servisinizi
yapabilirsiniz. Var olan tüm servislerin listesini almak için ``container:debug``
konsol komutu çalıştırın:

.. code-block:: bash

    php app/console container:debug

Daha fazla bilgi için :doc:`/book/service_container` kısmına bakın.

.. index::
   single: Controller; Hataları yönetmek
   single: Controller; 404 sayfaları

Hataları yönetmek ve 404 Sayfaları
----------------------------------
Bir şeyler bulunamadığında HTTP protokolinden 404 cevabı döndürülür. Bunu
yapmak için özel tipte bir istisna yaratmanız gereklidir. Eğer temel controller
sınıfından sınıfınızı türetirseniz bunu şu şekilde yaparsınız::

    public function indexAction()
    {
        $product = // veritabanından nesneyi al
        if (!$product) {
            throw $this->createNotFoundException('Ürün Bulunamadı');
        }

        return $this->render(...);
    }

``createNotFoundException()`` metodu Symfony içerisinde 404 HTTP cevabı
üreten özel bir ``NotFoundHttpException`` istisnası üretir.

Elbette controlleriniz içerisinde istediğiniz türde ``Exception`` sınıfını
kullanarak bir istisna üretebilirsiniz. Bunu Symfony2 otomatik olarak
500 HTTP response kodu ile döndürecektir.

.. code-block:: php

    throw new \Exception('Birşeyler Ters Gitti!');


Her durumda, stillendirilmiş, geliştiriciye hatayı bulmasına imkan sağlayan
bütün hata izleme bilgilerini içeren bir hata sayfası (eğer sayfa debug modunda ise) 
ve kullanıcıya genel bir hata sayfası gösterilecektir.
Bu hata sayfalarının ikiside istenilen şekilde düzenlenebilir. Daha fazla bilgi için
tarif kitabından ":doc:`/cookbook/controller/error_pages`" girdisini okuyun.

.. index::
   single: Controller; Oturum
   single: Oturum

Oturumları Yönetmek
--------------------
Symfony2 kullanıcı istekleri arasında (tarayıcı kullanan gerçek bir kullanıcı, 
bir bot, ya da bir web servisi) bilgileri saklayabileceğiniz güzel bir oturum nesnesi sağlar.
Varsayılan olarak Symfony2 özellikleri doğal PHP oturumlarını kullanarak bir çerez içerisinde
saklar.

Oturumdan bilgileri almak ya da saklamak herhangi bir controller içerisinden
kolaylıkla yapılabilir::

    $session = $this->getRequest()->getSession();

    // kullanıcının başka bir isteğinde kullanılmak üzere bir değer sakla
    $session->set('foo', 'bar');

    // başka bir istek için başka bir controller içerisinde
    $foo = $session->get('foo');

    // kullanıcı yerel bilgilerini sakla
    $session->setLocale('tr');

Bu nitelikler kullanıcının oturumu içerisinde aynı adlarla birlikte
oturum içerisinde kalacaktır.

.. index::
   single Session; Flash mesajları

Flash Mesajları
~~~~~~~~~~~~~~
Eğer isterseniz kullanıcının sadece bir isteği için saklanacak olan basit
mesajlarıda kullanıcı oturumunda saklayabilirsiniz. Bu form işlemede çok
kullanışlıdır. Örneğin istek için bir yönlendirme yapacak ve *sonraki* istekte
bir kısa mesaj göstereceksiniz. Bu tipteki mesajlara *flash mesajları* denmektedir.

Örneğin bir form gönderisini işlediğinizi düşünün::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->bindRequest($this->getRequest());
        if ($form->isValid()) {
            // bazı işlemler gerçekleştir.

            $this->get('session')->setFlash('notice', 'Değişiklikleriniz kayıt edildi!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

İstek işlendikten sonra controller ``notice`` adı altında bir flash mesajı
üretecek ve yönlendirme yapacaktır. (``notice``) adını koymanız çok önemli değildir.
Sizin bu mesajı nasıl adlandırdığınız farketmez.

Sonraki aksiyonun şablonunda aşağıdaki kod ``notice`` mesajını ekranda gösterecektir:

.. configuration-block::

    .. code-block:: html+jinja

        {% if app.session.hasFlash('notice') %}
            <div class="flash-notice">
                {{ app.session.flash('notice') }}
            </div>
        {% endif %}

    .. code-block:: php
    
        <?php if ($view['session']->hasFlash('notice')): ?>
            <div class="flash-notice">
                <?php echo $view['session']->getFlash('notice') ?>
            </div>
        <?php endif; ?>


Tasarım tarafında flash mesajları genelde sadece bir istek için yaşarlar
("Işığa kavuşurlar"). Bu örnekte yaptığınız gibi bunlar sadece bir
yönlendirme boyunca kullanılmak için tasarlanırlar.

.. index::
   single: Controller; Response (cevap) nesnesi

Response (cevap) Nesnesi
------------------------
Bir controller için tek gereklilik, bir ``Response`` nesnesi döndürmesidir.
:class:`Symfony\\Component\\HttpFoundation\\Response` sınıfı, kullanıcıya 
HTTP başlıkları ile doldurulup istemciye geri iletilmek üzere hazırlanan,
metin tabanlı mesajları yöneten ve HTTP response'u etrafında özetlenen işlemleri
yöneten bir PHP sınıfıdır:: 


    // 200 durum kodu ile birlikte basit bir Response üret(varsayılan)
    $response = new Response('Hello '.$name, 200);
    
    // 200 durum kodu ile bir JSON Response'u üret
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    ``headers`` değişkeni, ``Response`` başlıklarını yöneten ve değiştiren
    ve içerisinde bir çok faydalı metod bulunan :class:`Symfony\\Component\\HttpFoundation\\HeaderBag`
    nesnesinin değişkenidir.
    Başlık isimleri ``Content-Type`` 'ın eşiti olan ``content-type`` ya da 
    ``content_type`` isimlerini normalleştirir.

.. index::
   single: Controller; Request (istek) nesnesi

Request (istek) nesnesi
-----------------------
Routing yer tutucularının (placeholder) değerlerinin dışında controller ayrıca 
temel ``Controller`` sınıfından türetildiğinde, ``Request`` nesnesine de  erişebilir::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // bir Ajax isteğimi ?

    $request->getPreferredLanguage(array('en', 'tr'));

    $request->query->get('page'); // $_GET parametresini al

    $request->request->get('page'); // $_POST parametresini al

``Response`` nesnesi gibi request başlıkları da  kolaylıklar erişilebilecek
``HeaderBag`` nesnesinde tutulur.

Son Düşünceler
--------------
Ne zaman bir sayfa yaratsanız eninde sonuda bu sayfanın içeriğini yaratacak
olan kodu ve algoritmayı da yazacaksınız. Symfony'de bu, controller olarak adlandırılır
ve kullanıcı tarafına gönderilecek final ``Response`` nesnesini geri döndürmek
için herşeyi yapabilecek bir PHP fonksiyonudur.

Hayatı kolaylaştırmak için pek çok genel controller görevlerine erişmek için
sınfınızı temel ``Controller`` sınıfı ile genişletebilirsiniz. Örneğin
controller'ınız içerisine HTML kodu yazmak istemiyorsanız ``render()`` 
metodunu kullanarak içeriği bir şablondan ekrana bastırabilirsiniz.

Diğer bölümlerde controller'ın veri tabanından nasıl verileri aldığını, 
kayıt ettiğini, form verilerini işlediğini ve ön bellekleri (cache) işlediğini
göreceksiniz.

Tarif Kitabından Daha Fazlasını Öğrenin
---------------------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`
