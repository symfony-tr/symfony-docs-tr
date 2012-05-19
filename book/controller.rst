.. index::
   single: Controller

Controller
==========

Bir controller yaratmış olduğunuz HTTP isteğinden bilgiyi alarak bir HTTP
cevabı (Symfony2 'de ``Response`` nesnesi gibi) döndüren bir PHP fonksiyonudur.
Cevap bir HTML sayfası , bir XML dökümanı, serileştirilmiş bir JSON dizesi (array),
bir resim, bir yönlendirme (redirect), bir 404 hatası ya da düşünebildiğiniz
herhangi bir şey olabilir. Controller *uygulamanızın*  sayfa içeriğini oluşturmadaki
herhangi bir şeyi kapsayabilir. 

Bunun nasıl basit bir şey olduğunu görmek için  Symfony2 Controller'ini
uygulamada görelim.Aşağıdaki controller ekrana basitçe ``Hello world!`` 
yazacaktır::

    use Symfony\Component\HttpFoundation\Response;

    public function helloAction()
    {
        return new Response('Hello world!');
    }

Bir controller'in amacı her zaman aynıdır. Bir ``Response`` objesi döndürmek.
Bunu yaparken istekten bazı bilgileri okuyabilir, bir veritabanı kaynağını
çağırabilir, e-posta gönderebilir ya da kullanıcının  oturumuna herhangi
bir bilgi yazabilir. Fakat bütün bu durumlarda, controller sonuçta istemciye
geri gönderilmek üzere bir ``Response`` nesnesi döndürecektir.

Herhangi bir sihir yok ve diğer gereklilikler için endişelenmeyin!. Burada
bir kaç çok kullanılan örnek var:

* *Controller A* sitenin ana sayfasını temsil eden içeriği ``Response`` 
  nesnesi olarak hazırlar.

* *Controller B* Bu blog için veritabanından okunup gelecek değeri ekranda
  gösterebilmek için bir ``Response`` objesi yaratmak amacıyla , ``slug`` adındaki
  parametreyi okur. Eğer ``slug`` veritabanında bulunamazsa 404 durum 
  koduyla birlikte bir ``Response`` nesnesi yaratır.
  
* *Controller C* iletişim formunu işler. İstek (request) üzerinden gelen
  bilgileri okur ve iletişim bilgilerini webmastera göndermek için veritabanına 
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

#. Eşleşen route üzerinde belirtilen controller calıştırılır ve controller
   içerisindeki kod bir ``Response`` nesnesi yaratarak geri döndürür;

#. ``Response`` nesnesi içeriğindeki HTTP başlıkları istemciye geri döndürülür.

Sayfa yaratmak için controller yaratmak kadar basittir (#3) ve route yapmak URL'yi
bu controller ile eşleştirmek içindir (#2).

.. note::

    "front controller" isim olarak benzemesine rağmen biz bu bölümde
    "controller" 'lardan bahsedeceğiz.Bir front controller bütün istekleri
    yöneten, web klasöründe olan, kısa bir PHP dosyasıdır. Tipik bir uygulama
    bir production (imalat) front controllerine (örn: ``app.php``) ve 
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
*actions* olarak da bilinirler.


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
    kafanızı karıştırmasın. Bu tip bir uygulama sadece çeşitli controller/action
    'ları bir arada tutmak için kullanılır. Tipik olarak controller sınıfı pek çok
    controller/action'a ev sahipliği yapar (Örn. ``updateAction``, ``deleteAction``,
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
   single: Controller; Route'lar ve controllers

Controller için bir URI Eşleştirmek
------------------------------------
Yeni controller basit bir HTML sayfası döndürmekte. Gerçekte bu sayfayı görebilmeniz
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

şimdi  ``/hello/ryan`` olduğunda ``HelloController::indexAction()`` 
controlleri çalıştırılacak ve ``ryan`` değerini ``$name`` değişkenine
gönderecek. "Sayfa" yaratmanın anlamı basitçe bir controller metodu
yaratmak ve bunu bir route ile birleştirmektir.

Controller'i ifade eden yazıma ``AcmeHelloBundle:Hello:index`` dikkat edin.
Symfony2 farklı controllerları ifade edebilmek için esnek bir yazım sistemi
kullanır. Bu sık kullanılan yazım şekli Symfony2'ye ``AcmeHelloBundle`` olarak
adlandırılan bir bundle içerisindeki ``HelloController`` sınıfını çalıştırmasını
söyler. ``indexAction()`` metodu daha sonra çalıştırılır.

Farklı controller'lar için yazım şekli hakkında daha fazla bilgi almak için
:ref:`controller-string-syntax` belgesine bakın.

.. note::

    Bu örnekler için routing (yönlendirme) konfigürasyonlarının  yerleri direkt olarak
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

iki yer tutucu değişkeninin (``{first_name}``, ``{last_name}``) ve varsayılan
değeri atanmış olan ``color`` değişkeninin controller'in argümanları olduğuna
dikkat edin. Route eşleştiği zaman placeholde değişkenleri ``defaults`` ile
birleştirilir ve controllerda olan değişkenler için bir dize değişkenine çevrilir.

Route parametrelerini controller argümanları ile eşleştirmek kolay ve esnektir.
Sadece geliştirme süreci içerisinde şu kuralları aklınızıda tutun.

* **Controller'daki argümanların sırası önemli değidir.**

    Symfony route içerisindeki parametre isimleri ile controller'ın metodlarındaki
    argümanların adlarını eşleştirebilir. Diğer bir ifade ile ``{last_name}`` 
    parametresi ``$last_name`` argümanı ile eşleştirilir.  Controller'ın argümanları
    yeniden  
    Symfony is able to match the parameter names from the route to the variable
    names in the controller method's signature. In other words, it realizes that
    the ``{last_name}`` parameter matches up with the ``$last_name`` argument.
    Controller'ın argümanlarının tamamı yeniden sıralansa bile bu durum mükemmel
    çalışır::

        public function indexAction($last_name, $color, $first_name)
        {
            // ..
        }

* **Gerekli olan her controller argümanı bir route parametresi ile eşleşmelidir.**

    Aşağıdaki kod bir ``RuntimeException`` istisnası yaratacaktır. Çünki ``foo`` 
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

    Eğer, örneğin ``last_name`` controller'ınz için çok önemli değil ise, onu
    tamamen atlayabilirsiniz::

        public function indexAction($first_name, $color)
        {
            // ..
        }

.. tip::

    Her route (yönlendirme) özel bir ``_route`` parametresine sahiptir.Bu
    parametre eşleşen route 'un ismini tutar(örn: ``hello``). Bu 
    çok kullanışlı olmamasına rağmen bu bir controller argümanı
    olarak da kullanılabilir.

.. _book-controller-request-argument:

The ``Request`` as a Controller Argument
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For convenience, you can also have Symfony pass you the ``Request`` object
as an argument to your controller. This is especially convenient when you're
working with forms, for example::

    use Symfony\Component\HttpFoundation\Request;

    public function updateAction(Request $request)
    {
        $form = $this->createForm(...);
        
        $form->bindRequest($request);
        // ...
    }

.. index::
   single: Controller; Base controller class

The Base Controller Class
-------------------------

For convenience, Symfony2 comes with a base ``Controller`` class that assists
with some of the most common controller tasks and gives your controller class
access to any resource it might need. By extending this ``Controller`` class,
you can take advantage of several helper methods.

Add the ``use`` statement atop the ``Controller`` class and then modify the
``HelloController`` to extend it:

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

This doesn't actually change anything about how your controller works. In
the next section, you'll learn about the helper methods that the base controller
class makes available. These methods are just shortcuts to using core Symfony2
functionality that's available to you with or without the use of the base
``Controller`` class. A great way to see the core functionality in action
is to look in the
:class:`Symfony\\Bundle\\FrameworkBundle\\Controller\\Controller` class
itself.

.. tip::

    Extending the base class is *optional* in Symfony; it contains useful
    shortcuts but nothing mandatory. You can also extend
    ``Symfony\Component\DependencyInjection\ContainerAware``. The service
    container object will then be accessible via the ``container`` property.

.. note::

    You can also define your :doc:`Controllers as Services
    </cookbook/controller/service>`.

.. index::
   single: Controller; Common Tasks

Common Controller Tasks
-----------------------

Though a controller can do virtually anything, most controllers will perform
the same basic tasks over and over again. These tasks, such as redirecting,
forwarding, rendering templates and accessing core services, are very easy
to manage in Symfony2.

.. index::
   single: Controller; Redirecting

Redirecting
~~~~~~~~~~~

If you want to redirect the user to another page, use the ``redirect()`` method::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'));
    }

The ``generateUrl()`` method is just a helper function that generates the URL
for a given route. For more information, see the :doc:`Routing </book/routing>`
chapter.

By default, the ``redirect()`` method performs a 302 (temporary) redirect. To
perform a 301 (permanent) redirect, modify the second argument::

    public function indexAction()
    {
        return $this->redirect($this->generateUrl('homepage'), 301);
    }

.. tip::

    The ``redirect()`` method is simply a shortcut that creates a ``Response``
    object that specializes in redirecting the user. It's equivalent to:

    .. code-block:: php

        use Symfony\Component\HttpFoundation\RedirectResponse;

        return new RedirectResponse($this->generateUrl('homepage'));

.. index::
   single: Controller; Forwarding

Forwarding
~~~~~~~~~~

You can also easily forward to another controller internally with the ``forward()``
method. Instead of redirecting the user's browser, it makes an internal sub-request,
and calls the specified controller. The ``forward()`` method returns the ``Response``
object that's returned from that controller::

    public function indexAction($name)
    {
        $response = $this->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green'
        ));

        // further modify the response or return it directly
        
        return $response;
    }

Notice that the `forward()` method uses the same string representation of
the controller used in the routing configuration. In this case, the target
controller class will be ``HelloController`` inside some ``AcmeHelloBundle``.
The array passed to the method becomes the arguments on the resulting controller.
This same interface is used when embedding controllers into templates (see
:ref:`templating-embedding-controller`). The target controller method should
look something like the following::

    public function fancyAction($name, $color)
    {
        // ... create and return a Response object
    }

And just like when creating a controller for a route, the order of the arguments
to ``fancyAction`` doesn't matter. Symfony2 matches the index key names
(e.g. ``name``) with the method argument names (e.g. ``$name``). If you
change the order of the arguments, Symfony2 will still pass the correct
value to each variable.

.. tip::

    Like other base ``Controller`` methods, the ``forward`` method is just
    a shortcut for core Symfony2 functionality. A forward can be accomplished
    directly via the ``http_kernel`` service. A forward returns a ``Response``
    object::
    
        $httpKernel = $this->container->get('http_kernel');
        $response = $httpKernel->forward('AcmeHelloBundle:Hello:fancy', array(
            'name'  => $name,
            'color' => 'green',
        ));

.. index::
   single: Controller; Rendering templates

.. _controller-rendering-templates:

Rendering Templates
~~~~~~~~~~~~~~~~~~~

Though not a requirement, most controllers will ultimately render a template
that's responsible for generating the HTML (or other format) for the controller.
The ``renderView()`` method renders a template and returns its content. The
content from the template can be used to create a ``Response`` object::

    $content = $this->renderView('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

    return new Response($content);

This can even be done in just one step with the ``render()`` method, which
returns a ``Response`` object containing the content from the template::

    return $this->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

In both cases, the ``Resources/views/Hello/index.html.twig`` template inside
the ``AcmeHelloBundle`` will be rendered.

The Symfony templating engine is explained in great detail in the
:doc:`Templating </book/templating>` chapter.

.. tip::

    The ``renderView`` method is a shortcut to direct use of the ``templating``
    service. The ``templating`` service can also be used directly::
    
        $templating = $this->get('templating');
        $content = $templating->render('AcmeHelloBundle:Hello:index.html.twig', array('name' => $name));

.. index::
   single: Controller; Accessing services

Accessing other Services
~~~~~~~~~~~~~~~~~~~~~~~~

When extending the base controller class, you can access any Symfony2 service
via the ``get()`` method. Here are several common services you might need::

    $request = $this->getRequest();

    $templating = $this->get('templating');

    $router = $this->get('router');

    $mailer = $this->get('mailer');

There are countless other services available and you are encouraged to define
your own. To list all available services, use the ``container:debug`` console
command:

.. code-block:: bash

    php app/console container:debug

For more information, see the :doc:`/book/service_container` chapter.

.. index::
   single: Controller; Managing errors
   single: Controller; 404 pages

Managing Errors and 404 Pages
-----------------------------

When things are not found, you should play well with the HTTP protocol and
return a 404 response. To do this, you'll throw a special type of exception.
If you're extending the base controller class, do the following::

    public function indexAction()
    {
        $product = // retrieve the object from database
        if (!$product) {
            throw $this->createNotFoundException('The product does not exist');
        }

        return $this->render(...);
    }

The ``createNotFoundException()`` method creates a special ``NotFoundHttpException``
object, which ultimately triggers a 404 HTTP response inside Symfony.

Of course, you're free to throw any ``Exception`` class in your controller -
Symfony2 will automatically return a 500 HTTP response code.

.. code-block:: php

    throw new \Exception('Something went wrong!');

In every case, a styled error page is shown to the end user and a full debug
error page is shown to the developer (when viewing the page in debug mode).
Both of these error pages can be customized. For details, read the
":doc:`/cookbook/controller/error_pages`" cookbook recipe.

.. index::
   single: Controller; The session
   single: Session

Managing the Session
--------------------

Symfony2 provides a nice session object that you can use to store information
about the user (be it a real person using a browser, a bot, or a web service)
between requests. By default, Symfony2 stores the attributes in a cookie
by using the native PHP sessions.

Storing and retrieving information from the session can be easily achieved
from any controller::

    $session = $this->getRequest()->getSession();

    // store an attribute for reuse during a later user request
    $session->set('foo', 'bar');

    // in another controller for another request
    $foo = $session->get('foo');

    // set the user locale
    $session->setLocale('fr');

These attributes will remain on the user for the remainder of that user's
session.

.. index::
   single Session; Flash messages

Flash Messages
~~~~~~~~~~~~~~

You can also store small messages that will be stored on the user's session
for exactly one additional request. This is useful when processing a form:
you want to redirect and have a special message shown on the *next* request.
These types of messages are called "flash" messages.

For example, imagine you're processing a form submit::

    public function updateAction()
    {
        $form = $this->createForm(...);

        $form->bindRequest($this->getRequest());
        if ($form->isValid()) {
            // do some sort of processing

            $this->get('session')->setFlash('notice', 'Your changes were saved!');

            return $this->redirect($this->generateUrl(...));
        }

        return $this->render(...);
    }

After processing the request, the controller sets a ``notice`` flash message
and then redirects. The name (``notice``) isn't significant - it's just what
you're using to identify the type of the message.

In the template of the next action, the following code could be used to render
the ``notice`` message:

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

By design, flash messages are meant to live for exactly one request (they're
"gone in a flash"). They're designed to be used across redirects exactly as
you've done in this example.

.. index::
   single: Controller; Response object

The Response Object
-------------------

The only requirement for a controller is to return a ``Response`` object. The
:class:`Symfony\\Component\\HttpFoundation\\Response` class is a PHP
abstraction around the HTTP response - the text-based message filled with HTTP
headers and content that's sent back to the client::

    // create a simple Response with a 200 status code (the default)
    $response = new Response('Hello '.$name, 200);
    
    // create a JSON-response with a 200 status code
    $response = new Response(json_encode(array('name' => $name)));
    $response->headers->set('Content-Type', 'application/json');

.. tip::

    The ``headers`` property is a
    :class:`Symfony\\Component\\HttpFoundation\\HeaderBag` object with several
    useful methods for reading and mutating the ``Response`` headers. The
    header names are normalized so that using ``Content-Type`` is equivalent
    to ``content-type`` or even ``content_type``.

.. index::
   single: Controller; Request object

The Request Object
------------------

Besides the values of the routing placeholders, the controller also has access
to the ``Request`` object when extending the base ``Controller`` class::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // is it an Ajax request?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // get a $_GET parameter

    $request->request->get('page'); // get a $_POST parameter

Like the ``Response`` object, the request headers are stored in a ``HeaderBag``
object and are easily accessible.

Final Thoughts
--------------

Whenever you create a page, you'll ultimately need to write some code that
contains the logic for that page. In Symfony, this is called a controller,
and it's a PHP function that can do anything it needs in order to return
the final ``Response`` object that will be returned to the user.

To make life easier, you can choose to extend a base ``Controller`` class,
which contains shortcut methods for many common controller tasks. For example,
since you don't want to put HTML code in your controller, you can use
the ``render()`` method to render and return the content from a template.

In other chapters, you'll see how the controller can be used to persist and
fetch objects from a database, process form submissions, handle caching and
more.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/controller/error_pages`
* :doc:`/cookbook/controller/service`
