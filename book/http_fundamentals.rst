.. index::
   single: Symfony2 Temelleri

Symfony2 ve HTTP Temelleri
==============================
Tebrikler! Symfony2 hakkında öğrenerek, daha üretken, çok yönlü ve popüler 
(aslında, siz kendiniz en son kısımdasınız) bir web geliştiricisi olma 
yolunda doğru yol almaktasınız. 
Daha hızlı ve sağlam uygulamalar geliştirmek için Symfony2 temellere 
geri dönerek alışılmışın dışında geliştirme araçlarına sahiptir.
Symfony binlerce insanın yıllarca çalışmasının ürünü olan pek çok teknolojinin
en iyi fikirlerini ,yardımcı araçlarını ve konseptlerini bünyesinde barındırır.
Diğer bir kelimeyle "Symfony" öğrenirken web temellerini öğrenecek, en iyi
örneklerle geliştirme yapacak ve Symfony2 ile bağılmı ya da bağımsız olan 
pek çok inanılmaz PHP kütüphanesininde nasıl kullanılacağını öğreneceksiniz.
Şimdi başlayalım.

Web geliştirmede genel konsept olan HTTP konusunu Symfony2'nin felsefesine uygun olarak
açıklamaya başlayalım.
Genel bilgi düzeyiniz ya da tercih ettiğiniz programlama dilinden bağımsız olarak
bu kısmı herkezin **okuması gereklidir** .

HTTP Basittir
--------------
HTTP (geekler için Hypertext Transfer Protocol'ü demek)  iki makinenin birbiri
arasında konuşması için kullanılan metin tabanlı bil dildir. Hepsi bu kadar !.
Örneğin ne zaman en son `xkcd`_ karikatürlerini görmek istediğinizde şu (kabaca)
konuşma geçer:

.. image:: /images/http-xkcd.png
   :align: center

Kullandığınız gerçek dilde bu biraz daha karmaşık iken aslında iş temelde bu kadar basittir.
HTTP bu basit metin tabalı dilin kurallarını tanımlar. Http sizin nasıl web
geliştirme yaptığınızla ilgilenmez.Ama amacı sunucunuzun *her zaman* basit 
metin tabanlı istekleri anlayarak basit metin tabanlı cevaplar geri döndürmesini
sağlamaktır.
Symfony2'de işte bu durum üzerine inşaa edilmiştir. HTTP'yi nasıl kullandığınızın
önemi yoktur. Symfony2 ile HTTP konusunda nasıl uzmanlaşacağınızı öğreneceksiniz.

.. index::
   single: HTTP; İstek-cevap yaklaşımı

Adım 1: İstemci Bir İstek Gönderir
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Web'de her konuşma bir *istek (request)* ile başlar. İstek istemci (client)
tarafından (Örn: bir tarayıcı, bir iPhone uygulaması vb...) yaratılan 
HTTP olarak bilinen özel formatta bir metin tabanlı mesajdır. İstemci
sunucuya bir istek (request) gönderir ve cevabını bekler.

Birinci kısımdaki tarayıcı ve xkcd sunucusu arasındaki olaya geri dönelim:

.. image:: /images/http-xkcd-request.png
   :align: center


HTTP konuşmasında bu HTTP isteği aslında şuna benzemektedir:

.. code-block:: text

    GET / HTTP/1.1
    Host: xkcd.com
    Accept: text/html
    User-Agent: Mozilla/5.0 (Macintosh)


Bu basit mesaj istek yapan istemci hakkındaki bilgilerin neredeyse *tamamını*
kapsamaktadır. HTTP isteğinin ilk satırı çok önemli iki şeyi barındırır: URI 
ve HTTP metodu.

URI (örn: ``/``, ``/contact`` vb..) istemcinin istediği kaynağın benzersiz 
bir şekilde tanımlandığı konumu ifade eder. HTTP metodu (örn: ``GET``)
bu kaynakla ne *yapmak* istediğinizi ifade eder. HTTP metodları isteklerin
bir kaç genel bilinen yolla kaynak üzerinde nasıl davranılacağını gösteren
eylemlerdir :

+----------+---------------------------------------+
| *GET*    | Sunucudan kaynağı alır.               |
+----------+---------------------------------------+
| *POST*   | Sunucuda bir kaynak oluşturur.        |
+----------+---------------------------------------+
| *PUT*    | Sunucudaki kaynağı günceller          |
+----------+---------------------------------------+
| *DELETE* | Sunucudaki kaynağı siler              |
+----------+---------------------------------------+

Bu duruma göre belirlenen bir blog girdisinin silinmesini HTTP isteği ile
şu şekilde olabileceğini düşünebilirsiniz:

.. code-block:: text

    DELETE /blog/15 HTTP/1.1

.. note::

    Aslında HTTP tanımlamasında dokuz adet HTTP metodu tanımlanmış olmasına
    karşın pek çoğu artık kullanılmamakta ya da desteklenmemektedir.
    Gerçekte pek çok modern tarayıcı ``PUT`` ve ``DELETE`` metodlarına
    destek vermezler.

İlk satıra ek olarak bir HTTP isteği daima istek başlıklarının içerdiği
diğer tüm satırları içerir.Bu başlıklar istekte bulunan ``Host`` 'un
geniş çaptaki bilgilerin, istemcinin kabul ettiği cevap formatlarını 
(``Accept``) ve istemcinin istek yaptığı uygulama (``User-Agent``)
gibi pek çok bilgi içerir. Diğer başlıklar hakkındaki bilgiler
Wikipedia'nın `HTTP başlık alanları listesi`_ adlı makalesinde bulunmaktadır.

Adım 2: Sunucu bir cevap döndürür
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Sunucu bir istek alır almaz istemcinin hangi kaynağı istediği (URI aracılığı ile)
ve hangi istemcinin bu kaynakla ne yapmak istediğini (metod aracılığı ile) bilir.
Örneğin GET isteği durumunda sunucu kaynağı HTTP cevabı olarak hazırlar.  xkcd
web sunucusunu göz önüne aldığımızda :

.. image:: /images/http-xkcd.png
   :align: center

HTTP çevrilen cevap tarayıcıda şu şekilde gözükecektir:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 02 Apr 2011 21:05:05 GMT
    Server: lighttpd/1.4.19
    Content-Type: text/html

    <html>
      <!-- xkcd karikatürü için HTML kodu -->
    </html>


HTTP cevabı istenen kaynağın bilgisini barındırdığı gibi (bu örnekte HTML
içeriğidir) aynı zamanda cevap hakkında diğer bilgileri de barındırır.
İlk satır özellikle HTTP durum kodunu gösteren önemli bir kısımdır. (
Bu örnekte 200) Bu durum kodu ile dönüş yapılacak istemci arasında iletişim
kurulur. İstek başarılı oldumu ? Bir hata var mı ? Farklı bir durum kodu 
çıktığında bunun hata mı, başarılı bir istek olduğumu ya da istemcinin başka
bir şey istediğini mi (örneğin başka bir sayfaya yönlendirme) olduğu böylece
belirlenmiş olur. Durum kodları hakkındaki tam liste Wikipedia'nın 
`HTTP durum kodları`_  makalesinde bulunabilir.

İstekte olduğu gibi HTTP cevapları (response) HTTP başlıkları (headers)
olarak bilinen ayrıca ek bilgiler içerir. Örneğin önemli bir HTTP cevap
başlığı ``Content-Type`` dır. Aynı kaynağın içeriği HTML, XML, ya da  JSON 
olarak döndürülebilir. ``Content-Type`` başlığına  ``text/html`` gibi
Internet Medya tipleri bilgisi atanarak istemciye bu kaynağın nasıl 
döndürüleceği belirlenir.  Internet medya tipleri hakındaki tüm listeye
Wikipedia'nın `Genel medya tipleri listesi`_ makalesinden erişilebilir.

İçlerinde oldukça güçlü olan diğer başlık tanımlamalarıda mevcuttur. 
Örneğin bazı başlıklar güçlü bir önbellekleme (caching) sistemi için 
kullanılır.


İstekler, Cevaplar ve Web Geliştirme
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bu istek-cevap iletişimi web üzerindeki tüm iletişimin temelini sağlayan
bir süreçtir. Önemli olan şey, bu iletişimin çok basit
bir şekile ve oldukça güçlü olarak sağladığıdır.

Burada anlaşılması gereken en önemli şey sizin uygulama dilinden 
ve uygulama tipinizden (web, mobile, JSON API) ya da geliştirme mantalitenizden
bağımsız olarak bir uygulamanın **daima** her isteği anladığı ve uygun
bir cevabı geri döndürdüği durumu olmalıdır.

Symfony işte bu gerçek üzerine mimarilendirilmiştir.

.. tip::

    To learn more about the HTTP specification, read the original `HTTP 1.1 RFC`_
    or the `HTTP Bis`_, which is an active effort to clarify the original
    specification. A great tool to check both the request and response headers
    while browsing is the `Live HTTP Headers`_ extension for Firefox.

.. index::
   single: Symfony2 Temelleri; İstekler ve cevaplar

PHP'de İstekler ve cevaplar
-----------------------------
Peki PHP kullanarak "istekler" 'i nasıl yapacak ve "cevapları" nasıl 
yaratacağız?. Gerçekte PHP bu süreci kısa bir şekilde ifade eder:

.. code-block:: php

    <?php
    $uri = $_SERVER['REQUEST_URI'];
    $foo = $_GET['foo'];

    header('Content-type: text/html');
    echo 'The URI requested is: '.$uri;
    echo 'The value of the "foo" parameter is: '.$foo;

Bu acayip gözüken küçük uygulama aslında bir HTTP isteğini alıyor ve
buna göre bir HTTP cevabı yaratıyor.
HTTP 'nin ham mesajlarını işlemek yerine PHP ``$_SERVER`` ve ``$_GET``
adı verilen süper global değişkenleri ile isteğin tüm bilgilerini hazırlıyor.
Benzer olarak geri dönen HTTP formatlı metin mesajı ile ``header()`` 
fonksiyonunu kullanarak cevap başlıkları (reponse header) yaratarak basitçe
güncel içeriği gelen cevap mesajına göre ekrana basabiliyorsunuz. Aşağıda 
PHP istemciye gidecek gerçek bir HTTP cevabı yaratacak:

.. code-block:: text

    HTTP/1.1 200 OK
    Date: Sat, 03 Apr 2011 02:14:33 GMT
    Server: Apache/2.2.17 (Unix)
    Content-Type: text/html

    The URI requested is: /testing?foo=symfony
    The value of the "foo" parameter is: symfony

Symfony'de İstekler ve cevaplar
---------------------------------
Symfony, PHP'nin HTTP istekleri ve cevapları'nın arasındaki iletişimi
sağlamak için kullandığı yaklaşımı alternatif bir yok kullanarak kolaylıkla
gerçekleştirir. :class:`Symfony\\Component\\HttpFoundation\\Request` sınıfı
HTTP istek mesajlarının ifade edildiği basit bir sınıftır.
Bu sınıf ile isteğe (request) ait bilgileri parmaklarınızın ucunda olacaktır::

    use Symfony\Component\HttpFoundation\Request;

    $request = Request::createFromGlobals();

    // the URI being requested (e.g. /about) minus any query parameters
    $request->getPathInfo();

    // retrieve GET and POST variables respectively
    $request->query->get('foo');
    $request->request->get('bar', 'default value if bar does not exist');

    // retrieve SERVER variables
    $request->server->get('HTTP_HOST');

    // retrieves an instance of UploadedFile identified by foo
    $request->files->get('foo');

    // retrieve a COOKIE value
    $request->cookies->get('PHPSESSID');

    // retrieve an HTTP request header, with normalized, lowercase keys
    $request->headers->get('host');
    $request->headers->get('content_type');

    $request->getMethod();          // GET, POST, PUT, DELETE, HEAD
    $request->getLanguages();       // an array of languages the client accepts

``Request`` sınıfı arka tarafta çok iş yapar diye endişelenmeyin.
Örneğin ``isSecure()`` metodu kullanıcının güvenli bağlantı yapıp 
yapmadığını (Örn: ``https``) anlamak için sadece PHP'deki üç farklı 
değere bakar.

.. sidebar:: ParameterBags ve Request nitelikleri

    Yukarıda ``$_GET`` ve ``$_POST`` değişkenlerinin sırasıyla ``query`` 
    ve ``request`` özellikleri ile erişebildiğini gördünüz. Bu nesnelerin
    her birisi :class:`Symfony\\Component\\HttpFoundation\\ParameterBag`
    nesnesinin aşağıdaki gibi kullanılan metodlarıdır
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::get`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::has`,
    :method:`Symfony\\Component\\HttpFoundation\\ParameterBag::all` ve diğerleri.
    Aslında önceki örnekte kullanılan her bir özellikte ParameterBag'ın bir 
    özelliğidir.
    
    .. _book-fundamentals-attributes:
    
    Request sınıfı ayrıca ``attributes`` adındaki özelliği ile uygulamanın
    kendi içerisinde kullanılmak üzere bazı ekstra bilgileride tutar.
    Symfony2 framework'u için ``attributes`` içeriği,eşleşen yönlendirme
    için ``_controller`` bilgisi, ``id`` (eğer yönlendirme de ``{id}`` parametresi 
    kullandıysanız ) ve eşleşen yönlendirme ismi (``_route``) dir.
    
    ``attributes`` özelliği'nin tuttuğu bilgileri istediğiniz yerde kullanabilir
    ve isteğe göre içerik özel olarak tutabilirsiniz.
      

Symfony ayrıca ``Response`` adında HTTP response mesajları için bir sınıf barındırır. 
Bu uygulamanızda istemciye dönecek olan mesajları nesne tabanlı bir arabirimle 
inşa etmenize olanak sağlar::

    use Symfony\Component\HttpFoundation\Response;
    $response = new Response();

    $response->setContent('<html><body><h1>Hello world!</h1></body></html>');
    $response->setStatusCode(200);
    $response->headers->set('Content-Type', 'text/html');

    // prints the HTTP headers followed by the content
    $response->send();

Eğer Symfony başka bir şey teklif etseydi siz zaten bu request bilgisine 
ulaşıp cevap yaratmak için nesne yönelimli araç kullanacaktınız. Symfony
içerisinde gelen pek çok güçlü özellikten de öğrendiğiniz üzere ana amaç,
uygulamanızın *gelen isteği yorumlamak ve uygulama mantığınız içerisindeki en
uygun cevabı yaratmaktır.*

.. tip::

    ``Request`` ve ``Response`` sınıfları kendi başına çalışabilen ve Symfony'de
    ``HttpFoundation`` olarak adlandırılan bileşendedirler. Bu bileşen. 
    Symfony'den bağımsız olarak oturumlar ve dosya yüklemeleri içinde başka uygu
    lamalarda kullanılabilir.
    

İstekten Cevaba Bir Seyahat
------------------------------
HTTP'ninde olduğu gibi ``Request`` ve ``Response`` nesneleri oldukça basittir.
Bir uygulamanın geliştirilmesinin zorluğu nelerin gelip gittiğinin yazılmasıdır.
Diğer bir ifade ile gerçek çalışmalar istek bilgilerini yorumlar ve ilgili bilgi
yüklü cevapları oluşturur.

Uygulamanız muhtemelen e-posta göndermek form verilerini işlemek, veri tabanına
bilgi saklamak, HTML sayfalarını oluşturmak ve içeriğinizin güvenliğini sağlamak
gibi pek çok iş yapıyordur. Bunları ortak bir yapıda nasıl organize edip bakımını
sağlayabilirsiniz ?

Symfony ortadaki bu sorunları çözerek size bir şey bırakmaz.

Front Controller
~~~~~~~~~~~~~~~~~~~~

Geleneksel olarak uygulamalarda her sayfa bir dosya ile ifade edilir :

.. code-block:: text

    index.php
    contact.php
    blog.php


There are several problems with this approach, including the inflexibility
of the URLs (what if you wanted to change ``blog.php`` to ``news.php`` without
breaking all of your links?) and the fact that each file *must* manually
include some set of core files so that security, database connections and
the "look" of the site can remain consistent.

A much better solution is to use a :term:`front controller`: a single PHP
file that handles every request coming into your application. For example:

+------------------------+------------------------+
| ``/index.php``         | executes ``index.php`` |
+------------------------+------------------------+
| ``/index.php/contact`` | executes ``index.php`` |
+------------------------+------------------------+
| ``/index.php/blog``    | executes ``index.php`` |
+------------------------+------------------------+

.. tip::

    Using Apache's ``mod_rewrite`` (or equivalent with other web servers),
    the URLs can easily be cleaned up to be just ``/``, ``/contact`` and
    ``/blog``.

Now, every request is handled exactly the same. Instead of individual URLs
executing different PHP files, the front controller is *always* executed,
and the routing of different URLs to different parts of your application
is done internally. This solves both problems with the original approach.
Almost all modern web apps do this - including apps like WordPress.

Stay Organized
~~~~~~~~~~~~~~

But inside your front controller, how do you know which page should
be rendered and how can you render each in a sane way? One way or another, you'll need to
check the incoming URI and execute different parts of your code depending
on that value. This can get ugly quickly:

.. code-block:: php

    // index.php

    $request = Request::createFromGlobals();
    $path = $request->getPathInfo(); // the URI path being requested

    if (in_array($path, array('', '/')) {
        $response = new Response('Welcome to the homepage.');
    } elseif ($path == '/contact') {
        $response = new Response('Contact us');
    } else {
        $response = new Response('Page not found.', 404);
    }
    $response->send();

Solving this problem can be difficult. Fortunately it's *exactly* what Symfony
is designed to do.

The Symfony Application Flow
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When you let Symfony handle each request, life is much easier. Symfony follows
the same simple pattern for every request:

.. _request-flow-figure:

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 request flow

   Incoming requests are interpreted by the routing and passed to controller
   functions that return ``Response`` objects.

Each "page" of your site is defined in a routing configuration file that
maps different URLs to different PHP functions. The job of each PHP function,
called a :term:`controller`, is to use information from the request - along
with many other tools Symfony makes available - to create and return a ``Response``
object. In other words, the controller is where *your* code goes: it's where
you interpret the request and create a response.

It's that easy! Let's review:

* Each request executes a front controller file;

* The routing system determines which PHP function should be executed based
  on information from the request and routing configuration you've created;

* The correct PHP function is executed, where your code creates and returns
  the appropriate ``Response`` object.

A Symfony Request in Action
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Without diving into too much detail, let's see this process in action. Suppose
you want to add a ``/contact`` page to your Symfony application. First, start
by adding an entry for ``/contact`` to your routing configuration file:

.. code-block:: yaml

    contact:
        pattern:  /contact
        defaults: { _controller: AcmeDemoBundle:Main:contact }

.. note::

   This example uses :doc:`YAML</components/yaml>` to define the routing
   configuration. Routing configuration can also be written in other formats
   such as XML or PHP.

When someone visits the ``/contact`` page, this route is matched, and the
specified controller is executed. As you'll learn in the :doc:`routing chapter</book/routing>`,
the ``AcmeDemoBundle:Main:contact`` string is a short syntax that points to a
specific PHP method ``contactAction`` inside a class called ``MainController``:

.. code-block:: php

    class MainController
    {
        public function contactAction()
        {
            return new Response('<h1>Contact us!</h1>');
        }
    }

In this very simple example, the controller simply creates a ``Response``
object with the HTML "<h1>Contact us!</h1>". In the :doc:`controller chapter</book/controller>`,
you'll learn how a controller can render templates, allowing your "presentation"
code (i.e. anything that actually writes out HTML) to live in a separate
template file. This frees up the controller to worry only about the hard
stuff: interacting with the database, handling submitted data, or sending
email messages. 

Symfony2: Build your App, not your Tools.
-----------------------------------------

You now know that the goal of any app is to interpret each incoming request
and create an appropriate response. As an application grows, it becomes more
difficult to keep your code organized and maintainable. Invariably, the same
complex tasks keep coming up over and over again: persisting things to the
database, rendering and reusing templates, handling form submissions, sending
emails, validating user input and handling security.

The good news is that none of these problems is unique. Symfony provides
a framework full of tools that allow you to build your application, not your
tools. With Symfony2, nothing is imposed on you: you're free to use the full
Symfony framework, or just one piece of Symfony all by itself.

.. index::
   single: Symfony2 Components

Standalone Tools: The Symfony2 *Components*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

So what *is* Symfony2? First, Symfony2 is a collection of over twenty independent
libraries that can be used inside *any* PHP project. These libraries, called
the *Symfony2 Components*, contain something useful for almost any situation,
regardless of how your project is developed. To name a few:

* `HttpFoundation`_ - Contains the ``Request`` and ``Response`` classes, as
  well as other classes for handling sessions and file uploads;

* `Routing`_ - Powerful and fast routing system that allows you to map a
  specific URI (e.g. ``/contact``) to some information about how that request
  should be handled (e.g. execute the ``contactAction()`` method);

* `Form`_ - A full-featured and flexible framework for creating forms and
  handling form submissions;

* `Validator`_ A system for creating rules about data and then validating
  whether or not user-submitted data follows those rules;

* `ClassLoader`_ An autoloading library that allows PHP classes to be used
  without needing to manually ``require`` the files containing those classes;

* `Templating`_ A toolkit for rendering templates, handling template inheritance
  (i.e. a template is decorated with a layout) and performing other common
  template tasks;

* `Security`_ - A powerful library for handling all types of security inside
  an application;

* `Translation`_ A framework for translating strings in your application.

Each and every one of these components is decoupled and can be used in *any*
PHP project, regardless of whether or not you use the Symfony2 framework.
Every part is made to be used if needed and replaced when necessary.

The Full Solution: The Symfony2 *Framework*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

So then, what *is* the Symfony2 *Framework*? The *Symfony2 Framework* is
a PHP library that accomplishes two distinct tasks:

#. Provides a selection of components (i.e. the Symfony2 Components) and
   third-party libraries (e.g. ``Swiftmailer`` for sending emails);

#. Provides sensible configuration and a "glue" library that ties all of these
   pieces together.

The goal of the framework is to integrate many independent tools in order
to provide a consistent experience for the developer. Even the framework
itself is a Symfony2 bundle (i.e. a plugin) that can be configured or replaced
entirely.

Symfony2 provides a powerful set of tools for rapidly developing web applications
without imposing on your application. Normal users can quickly start development
by using a Symfony2 distribution, which provides a project skeleton with
sensible defaults. For more advanced users, the sky is the limit.

.. _`xkcd`: http://xkcd.com/
.. _`HTTP 1.1 RFC`: http://www.w3.org/Protocols/rfc2616/rfc2616.html
.. _`HTTP Bis`: http://datatracker.ietf.org/wg/httpbis/
.. _`Live HTTP Headers`: https://addons.mozilla.org/en-US/firefox/addon/3829/
.. _`HTTP durum kodları`: http://en.wikipedia.org/wiki/List_of_HTTP_status_codes
.. _`HTTP başlık alanları listesi`: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields
.. _`Genel medya tipleri listesi`: http://en.wikipedia.org/wiki/Internet_media_type#List_of_common_media_types
.. _`HttpFoundation`: https://github.com/symfony/HttpFoundation
.. _`Routing`: https://github.com/symfony/Routing
.. _`Form`: https://github.com/symfony/Form
.. _`Validator`: https://github.com/symfony/Validator
.. _`ClassLoader`: https://github.com/symfony/ClassLoader
.. _`Templating`: https://github.com/symfony/Templating
.. _`Security`: https://github.com/symfony/Security
.. _`Translation`: https://github.com/symfony/Translation
