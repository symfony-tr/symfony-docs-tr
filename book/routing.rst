.. index::
   single: Routing (Yönlendirme)

Routing (Yönlendirme)
=====================
Güzel URL'ler herhangi bir ciddi web uygulamasının olmazsa olmazıdır.
Bunun anlamı ``index.php?article_id=57`` gibi biçimsiz URL'lerin arkasından 
ayrılıp ``/read/intro-to-symfony`` gibi URL'lere izin vermeniz gerekililiğidir.

Esnek olmak oldukça önemli. Farz edelim bir sayfanın URL'sini ``/blog`` dan
``/news`` 'a çevireceksiniz. Bu değişikliği yapmanız için kaç tane linki
arayıp bulmanız gerekiyor? Eğer Symfony'nin router 'ını kullanıyorsanız değiştirmek
basit.

Symfony2 router'ı size uygulamanızın farklı alanları için yaratıcı URL'ler
tanımlamanıza olanak sağlar. Bu bölümün sonunda size bunları yapabileceksiniz:

* Controller'ları eşleyen karmaşık route'lar
* Controller'lar ve Şablonlar içerisinde URL'ler yaratmak
* Bundle'lardan (ya da herhangi bir yerden) route kaynaklarını çağırmak 
* route'lar üzerinde hataları ayıklamak.

.. index::
   single: Routing; Temeller

Uygulamada Route'lar
--------------------
Bir *route* (yönlendirme) controller'a işaret eden bir URL adresidir. 
Örneğin varsayalım ki ``/blog/my-post`` ya da ``/blog/all-about-symfony``
gibi bir URL'yi eşlemek ve controller'a göndermek daha sonrada bu blog girdisini
araştırıp ekrana basmak istiyorsunuz.
Route basit:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

``blog_show`` olarak ifade edilen route şablonu ``slug`` olarak verilen
alanda ki değere göre  ``/blog/*`` joker'i gibi davranır.
``/blog/my-blog-post`` URL'si için controller'da kullanacağınız şekilde 
``slug`` değişkeninin değeri ``my-blog-post`` olacaktır (okumaya devam edin).

``_controller`` parametresi özel bir anahtardır ve Symfony'e URL ile eşleşme
sağlandığında hangi controller'in çalışacağını söyler. ``_controller`` içindeki 
string değeri :ref:`mantıksal isim<controller-string-syntax>` olarak adlandırılır.
Devam eden kalıp özel bir PHP sınıfı ve metodunu işaret eder:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    
    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            $blog = // $slug değişkenini veritabanı sorgusu için kullan
            
            return $this->render('AcmeBlogBundle:Blog:show.html.twig', array(
                'blog' => $blog,
            ));
        }
    }

Tebrikler! İlk route'unuzu yarattınız ve onu bir controller'a bağladınız.
Şimdi ``/blog/my-post`` adresini ziyaret ettiğinizde ``showAction``
controlleri çalışacak ve ``$slug`` değişkeninin değeri ``my-post`` olacak.

Symfony2 router'inin amacı isteğin URL'sini bir controller ile eşlemektir.
Bu arada karmaşık URL'leri kolayca yapmak için pek çok ipucu ve kısayol
öğreneceksiniz.

.. index::
   single: Routing; Kaputun altında

Routing: Kaputun altında
-------------------------
Uygulamanıza bir istek yaptığınızda istemcinin istekte bulunduğu bir "kaynağı"
tam olarak barındıran bir adresi gönderirsiniz. Bu adres URL (yada URI) olarak
adlandırılır ve ``/contact``, ``/blog/read-me`` ya da herhangi bir şekilde
olabilir. Şu HTTP isteğine örnek olarak bakalım:

.. code-block:: text

    GET /blog/my-blog-post

Symfony2 routing sisteminin amacı bu URL'yi yorumlamak ve hangi controller'in
çalışacağını belirlemektir. Bütün süreç şu şekildedir:

#. İstek (request) Symfony2 front controller'i tarafından işlenir (örn. ``app.php``);

#. Symfony2 çekirdeği (örn. Kernel) isteği denetlemesi için router'a görev verir.
 
#. Router gelen URL yi belirli bir route ile eşleştirir ve bu route hakkında hangi
   controller'in çalışacağını içeren bir bilgi döndürür;
   
#. Symfony2 Kernel'i controlleri çalıştırır ve en sonunda bir ``Response``
   nesnesi döndürür.
   

.. figure:: /images/request-flow.png
   :align: center
   :alt: Symfony2 request flow

   Routing katmanı gelen URL'leri belirli controllerların çalışması için 
   çeviren yardımcı bir araçtır.

.. index::
   single: Routing; route yaratmak

Route Yaratmak
---------------
Symfony uygulamanız için tüm route 'ları tek bir route konfigürasyon dosyasından
yükler. Bu dosya genellikle ``app/config/routing.yml`` dosyasıdır ancak herhangi
bir formattada (XML ya da PHP dosyası gibi) olabilir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            # ...
            router:        { resource: "%kernel.root_dir%/config/routing.yml" }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config ...>
            <!-- ... -->
            <framework:router resource="%kernel.root_dir%/config/routing.xml" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array(
            // ...
            'router'        => array('resource' => '%kernel.root_dir%/config/routing.php'),
        ));

.. tip::

    Tüm route bilgileri tek bir dosyadan yüklenmesine rağmen genel uygulama
    farklı route dosyalarını dışarıdan bu dosyaya eklemek şeklindedir. 
    :ref:`routing-include-external-resources` kısmını okuyarak daha fazla
    bili alabilirsiniz.

Temel Route Konfigürasyonu
~~~~~~~~~~~~~~~~~~~~~~~~~

Bir route tanımlamak kolaydır ve tipik bir uygulama pek çok route'u barındırır.
Temel bir route iki parçadan oluşur: ``pattern``  ve eşleme için ``defaults`` 
dize (array) si:

.. configuration-block::

    .. code-block:: yaml

        _welcome:
            pattern:   /
            defaults:  { _controller: AcmeDemoBundle:Main:homepage }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="_welcome" pattern="/">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
            </route>

        </routes>

    ..  code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('_welcome', new Route('/', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
        )));

        return $collection;


Bu route ana sayfayı eşler (``/``) ve ``AcmeDemoBundle:Main:homepage`` controlleri
ile eşleştirir. ``_controller`` stringi Symfony tarafından çalıştırılacak gerçek
bir PHP fonksiyonuna çevrilir. Bu süreç :ref:`controller-string-syntax` kısmında
kısaca açıklanmıştır.

.. index::
   single: Routing; Placeholder'lar (Yertutucular)

Placeholderlar (Yertutucu) ile Route
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Elbette route sistemi pek çok ilginç route'u barındırır. Çoğu route 
bir ya da daha fazla "wildcard" olarak adlandırılan yertutucu içerecektir:

.. configuration-block::

    .. code-block:: yaml

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog_show', new Route('/blog/{slug}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Kalıp ``/blog/*``şekilde karşılayan herhangi bir şey olacaktır. Daha da iyisi
``{slug}`` placeholder 'ı ile eşleşen değer controller'iniz içerisinde olacaktır.
Diğer bir ifade eile eğer URL ``/blog/hello-world`` ise ``$slug`` değişkeninin
değeri olan ``hello-world`` değeri controller içerisine aktarılır.Bu örneğin 
bu stringi karşılayan blog girdisini bulmak için kullanılabilir.

Kalıp basitçe ``/blog`` girdisi ile *eşleşmeyecektir*. Bunun nedeni
yer tutucularının tamamının içerisinde verinin gerekli olmasıdır. Bu 
``defaults`` arrayına bir yer tutucu ekleyerek değiştirlebilir.

Zorunlu ve Seçilmlik Placeholderlar 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bir şeyleri daha heyecan verici yapmak için hayali blog uygulamamızda
tüm blog girdilerini gösteren yeni bir route ekleyelm:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Şimdiye kadar bu route mümkün olabildiği kadar basitti. Herhangi bir
place holder içermiyordu ve sadece ``/blog`` URL si ile eşleşiyordu. Fakat
farzedelim bu route'un sayfalamaya (pagination) gereksininimi varsa. 
Örneğin ``/blog/2`` blog girdilerinin ikinci sayfasını gösterecekse?
Route'u yeni bir ``{page}`` placeholder 'ı ile güncelleyelim:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
        )));

        return $collection;

Önceki ``{slug}`` placeholder'u gibi controller içerisinde ``{page}``
place holder'ının değeride kullanılacak. Bu değer aynı zamanda hangi 
blog girdilerinin sayfada gösterileceğini belirlemekte de kullanılabilir.

Fakat durun!. placeholder değerleri varsayılan olarak gerekliydi. Bu route
basitçe ``/blog`` URL'sini eşleştirmeyecek. Bunun blog içerisinde sayfa 1'i 
görmek için ``/blog/1`` URL'sini kullanmak zorundayız! Zengin Web Uygulamalarında 
bundan başka bir yol olmamasına rağmen ``{page}`` parametresini seçimlik yapmayı
sağlayabiliriz. Bu ``defaults`` kolleksiyonunun içerisinde şu şekilde olabilir:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        return $collection;


``defaults`` anahtarı içerisinde ``page`` adıyla parametre atanması ile ``{page}`` placeholder'ını 
kullanmamıza gerek kalmadı. ``/blog`` URL'si bu route eşleştiğinde ``page`` parametresi
``1`` değerine sahip olacak. ``/blog/2`` URL eşlemesi geldiğinde ise ``page`` 
parametresini ``2`` yapacak. Mükemmel.

+---------+------------+
| /blog   | {page} = 1 |
+---------+------------+
| /blog/1 | {page} = 1 |
+---------+------------+
| /blog/2 | {page} = 2 |
+---------+------------+

.. index::
   single: Routing; Gereklilikler

Koşul (Requirements) Eklemek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hızlıca route'ların nasıl yaratılığına bakalım:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }

        blog_show:
            pattern:   /blog/{slug}
            defaults:  { _controller: AcmeBlogBundle:Blog:show }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
            </route>

            <route id="blog_show" pattern="/blog/{slug}">
                <default key="_controller">AcmeBlogBundle:Blog:show</default>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        )));

        $collection->add('blog_show', new Route('/blog/{show}', array(
            '_controller' => 'AcmeBlogBundle:Blog:show',
        )));

        return $collection;

Problemi görebildiniz mi?. Dikkat ederseniz iki route'un ``/blog/*`` şeklinde
eşleşen kalıpları var. Symfony router'ı buldukları içerisinde 
her zaman **ilk** eşleşen route'u seçer. Diğer bir ifade ile ``blog_show``
route'u *asla* eşleşmeyecektir. Bunun yerine ``/blog/my-blog-post`` değeri
ilk route (``blog``) ile eşleşecek ve ``my-blog-post`` 'un ``{page}`` 
parametresi için herhangi bir anlamı olmayacaktır.

+--------------------+-------+-----------------------+
| URL                | route | parameters            |
+====================+=======+=======================+
| /blog/2            | blog  | {page} = 2            |
+--------------------+-------+-----------------------+
| /blog/my-blog-post | blog  | {page} = my-blog-post |
+--------------------+-------+-----------------------+

Problemin cevabı route 'a bir *koşul* (requirements) eklemektir.
Bu örnekteki route'ları eğer ``/blog/{page}`` kalıbında ``{page}``
kısmı *sadece* integer bir değer olarak gelirse mükemmel olarak çalışacaktır. 
Çok şükür ki düzenli ifadelerden oluşan koşullar her parametreye
kolaylıkla eklenebilir. Örneğin:

.. configuration-block::

    .. code-block:: yaml

        blog:
            pattern:   /blog/{page}
            defaults:  { _controller: AcmeBlogBundle:Blog:index, page: 1 }
            requirements:
                page:  \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="blog" pattern="/blog/{page}">
                <default key="_controller">AcmeBlogBundle:Blog:index</default>
                <default key="page">1</default>
                <requirement key="page">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('blog', new Route('/blog/{page}', array(
            '_controller' => 'AcmeBlogBundle:Blog:index',
            'page' => 1,
        ), array(
            'page' => '\d+',
        )));

        return $collection;

``\d+``  koşulu ``{page}`` parametresinin mutlaka bir dijit (örn. bir sayı)
olmasını söyleyen bir düzenli ifadedir.  ``blog`` route'u hala ``/blog/2``
(2 çünkü bir sayıdır) gibi bir URL 'yi eşlemektedir. Fakat ``/blog/my-blog-post``
gibi bir URL'nin herhangi bir anlamı yoktur(çünkü ``my-blog-post`` bir sayı
*değildir*).
Eğer ``/blog/my-blog-post`` gibi bir URL gelirse bunun sonucunda ``blog_show``  
doğru bir şekilde eşleşecektir.

+--------------------+-----------+-----------------------+
| URL                | route     | parameters            |
+====================+===========+=======================+
| /blog/2            | blog      | {page} = 2            |
+--------------------+-----------+-----------------------+
| /blog/my-blog-post | blog_show | {slug} = my-blog-post |
+--------------------+-----------+-----------------------+

.. sidebar:: Önceki Route'lar daima Kazanır

    Bunların elbetteki tümünün anlamı route'ların sıralaması oldukça önemlidir.
    Eğer ``blog_show`` route 'u ``blog``  dan önce gelirse ``/blog/2`` URL'si
    ``blog`` yerine ``blog_show`` eşleşeceğinden ``blog_show`` 'un ``{slug}``
    parametresi uygun koşula sahip olmayacaktır. Uygun sıralamalar ve zekice 
    düzenlenen koşullarla herşeyi başarabilirsiniz.

Parametre koşullarının düzenli ifadeler olması sayesinde her koşulun karmaşıklığı
ve esnekliği tamamen size bağlıdır. Varsayalımki uygulamanızın ana sayfası iki
dilde ve URL'ye bağlı olarak bunların değişmesi gerekiyor:

.. configuration-block::

    .. code-block:: yaml

        homepage:
            pattern:   /{culture}
            defaults:  { _controller: AcmeDemoBundle:Main:homepage, culture: en }
            requirements:
                culture:  en|tr

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="homepage" pattern="/{culture}">
                <default key="_controller">AcmeDemoBundle:Main:homepage</default>
                <default key="culture">en</default>
                <requirement key="culture">en|tr</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/{culture}', array(
            '_controller' => 'AcmeDemoBundle:Main:homepage',
            'culture' => 'en',
        ), array(
            'culture' => 'en|tr',
        )));

        return $collection;

Gelen isteğin URL'sindeki ``{culture}`` kısmı ``(en|tr)`` 'ye dayalı olarak
eşleşir.

+-----+-----------------------------+
| /   | {culture} = en              |
+-----+-----------------------------+
| /en | {culture} = en              |
+-----+-----------------------------+
| /tr | {culture} = tr              |
+-----+-----------------------------+
| /es | *bu route ile eşleşmeyecek* |
+-----+-----------------------------+

.. index::
   single: Routing; Metod şartları

HTTP Method şartı eklemek.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

URL'ye ek olarak ayrıca gelen isteğin *metodu* 'nu da eşleyebilirsiniz
(örn: GET, HEAD, POST, PUT, DELETE). Varsayalım bir tanesi formu gösteren
(GET isteğinde) bir tanesi de form submit edildiğinde onu işleyen (POST
isteğinde) iki adet controller'dan oluşan bir iletişim formunuz var.Bu
aşağıdaki route konfigürasyonu ile halledilebilir:

.. configuration-block::

    .. code-block:: yaml

        contact:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contact }
            requirements:
                _method:  GET

        contact_process:
            pattern:  /contact
            defaults: { _controller: AcmeDemoBundle:Main:contactProcess }
            requirements:
                _method:  POST

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="contact" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contact</default>
                <requirement key="_method">GET</requirement>
            </route>

            <route id="contact_process" pattern="/contact">
                <default key="_controller">AcmeDemoBundle:Main:contactProcess</default>
                <requirement key="_method">POST</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('contact', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contact',
        ), array(
            '_method' => 'GET',
        )));

        $collection->add('contact_process', new Route('/contact', array(
            '_controller' => 'AcmeDemoBundle:Main:contactProcess',
        ), array(
            '_method' => 'POST',
        )));

        return $collection;

Bu iki route'un benzer kalıpları (``/contact``) olmasına rağmen, ilk route
sadece GET isteklerinde eşleşecek diğeri ise sadece POST isteklerinde eşleşecektir.
Bunun anlamı form'u ve gösterme işlemlerini aynı URL üzerinden gösterirken
bunların işlemesi aynı controller'in iki farklı metodu (aksiyonu) tarafından
işletilmektedir.

.. note::
    Eğer  ``_method`` koşlulu belirtilmeyidse route *tüm* metodları eşleştirecektir.


Diğer koşullar gibi ``_method`` koşulu düzenli ifadeyi yorumlar. ``GET`` *ya da * ``POST``
istekleri için ``GET|POST`` ifadesini kullanabilirsiniz.

.. index::
   single: Routing; Gelişmiş Örnek
   single: Routing; _format parametresi

.. _advanced-routing-example:

İleri Route Örneği
~~~~~~~~~~~~~~~~~~

Bu noktada Symfony'de güçlü bir route yapısını yapabilmek için herşeye
sahipiniz. Aşağıdaki örnek routing sisteminin ne kadar esnek olabileceğini
göstermektedir:

.. configuration-block::

    .. code-block:: yaml

        article_show:
          pattern:  /articles/{culture}/{year}/{title}.{_format}
          defaults: { _controller: AcmeDemoBundle:Article:show, _format: html }
          requirements:
              culture:  en|tr
              _format:  html|rss
              year:     \d+

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="article_show" pattern="/articles/{culture}/{year}/{title}.{_format}">
                <default key="_controller">AcmeDemoBundle:Article:show</default>
                <default key="_format">html</default>
                <requirement key="culture">en|tr</requirement>
                <requirement key="_format">html|rss</requirement>
                <requirement key="year">\d+</requirement>
            </route>
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('homepage', new Route('/articles/{culture}/{year}/{title}.{_format}', array(
            '_controller' => 'AcmeDemoBundle:Article:show',
            '_format' => 'html',
        ), array(
            'culture' => 'en|tr',
            '_format' => 'html|rss',
            'year' => '\d+',
        )));

        return $collection;

Gördüğünüz gibi route sadece ``{culture}`` kısmını ``en`` ya da ``tr`` olmasına
göre ve eğer ``{year}`` bir sayı ise eşleyecektir. Bu route ayrıca yer tutucuların
aralarına bölü ("/") işareti koyarak nasıl ard arda kullanılabileceğini de 
göstermektedir. Eşleşen URL'ler şu şekilde olabilir:

* ``/articles/en/2010/my-post``
* ``/articles/fr/2010/my-post.rss``

.. _book-routing-format-param:

.. sidebar:: Özel ``_format`` Routing Parametresi

    Bu örnek aynı zamanda ``_format`` özel parametresini işaret etmektedir.
    Bu parametre kullanıldığında eşleşen değerler ``Request`` nesnesinin
    "request format" değeri haline dönerler. Eninde sonunda istek formatı
    cevabın ``Content-Type`` değerini set etmek için kullanılır.
    (Örn. bir ``json`` istek formatı  ``Content-Type`` içindeki 
    ``application/json`` değerine çevrilir).
    
    Ayrıca controller'in farklı şablonlarını ekrana basmak için de  ``_format``
    kullanılır.  ``_format`` parametresi aynı içeriğin farklı formatlarda
    ekrana basılması için oldukça güçlü bir çözümdür.

Özel Route Parametreleri
~~~~~~~~~~~~~~~~~~~~~~~~~~
Gördüğünüz gibi her route parametresi ya da varsayılan değeri açıkça bir 
controller metodunda argüman olarak kullanılabiliyor. Bunlarla birlikte 
herbirisi uygulamanızda benzersiz özelliğe sahip olacak üç adet parametreside
bulunmaktadır.

* ``_controller``: Gördüğünüz gibi,bu parametre route eşleştiğinde hangi
  controller'in çalışacağı bilgisini tutar.

* ``_format``: istek formatını ayarlamada kullanılır 
  (:ref:`Daha fazlası için <book-routing-format-param>`);

* ``_locale``: oturumun yerel ayarlarını (locale) yapmakta kullanılır 
  (:ref:`Daha fazlası için <book-translation-locale-url>`);

.. index::
   single: Routing; Controller'lar
   single: Controller; String adlandırma formatı

.. _controller-string-syntax:

Controller Adlandırma Şablonu
------------------------------

Her route'un route eşleşmesi halinde çalıştırılacak controller'in belirlenmesi
amacıyla bir ``_controller`` parametresi olmalıdır. Bu parametre Symfony'nin
belirli bir PHP metod ve sınıfını eşleyebilmesi için *mantıksal controller adı*
olarak adlandırılan bir string şablonu ile kullanılır. Bu şablon üç parçaya
sahiptir ve her parça iki nokta üstüste (:) karakteri ile ayrılır:

    **bundle**:**controller**:**aksiyon**

Örneğin, ``AcmeBlogBundle:Blog:show`` şeklinde bir ``_controller`` 
değerinin anlamı:

+----------------+-------------------+-------------+
| Bundle         | Controller Sınıfı | Method Adı  |
+================+===================+=============+
| AcmeBlogBundle | BlogController    | showAction  |
+----------------+-------------------+-------------+

Controller şu şekilde gözükebilir:

.. code-block:: php

    // src/Acme/BlogBundle/Controller/BlogController.php
    
    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    
    class BlogController extends Controller
    {
        public function showAction($slug)
        {
            // ...
        }
    }

Dikkat ederseniz Symfony stringe ``Controller`` için sınıf ismi 
(``Blog`` => ``BlogController``) ve ``Aksiyon`` 'na da metod ismini 
(``show`` => ``showAction``) ekler.

Eğer isterseniz bu controller'i sınıfı ve metodu tam olarak gösterilen
``Acme\BlogBundle\Controller\BlogController::showAction`` şekli ile de 
kullanabilirsiniz.
Eğer aşağılarda ki örneklerde kullanılan yazım şekillerine bakarsanız 
mantıksal isminin daha kısa ve esnek olduğunu görürsünüz.

.. note::

   Mantıksal isme ya da tam sınıf ismine ek olarak Symfony controller'a
   referans verme de üçüncü bir yol kullanır. Bu metod sadece ikinokta
   üstüste ayracını bir kere kullanmaktır (Örn:``service_name:indexAction``).
   Bu kullanım controller'i bir servis gibi referans verir
   (bkz :doc:`/cookbook/controller/service`).

Route Parametreleri ve Controller Argümanları
---------------------------------------------

Route parametreleri (Örn. ``{slug}``) her controller metodunun
bir argümanı olarak yapıldıkları için ayrıca önemlidir:

.. code-block:: php

    public function showAction($slug)
    {
      // ...
    }

Gerçekte ``defaults`` kolleksiyonunun tamamı bir tek dize değişkeni
değeri ile parametre değerlerini birleşimidir. Bu array içerisindeki
her bir anahtar controller'in bir argümanını işaret eder.

Diğer bir ifade ile confroller metodunun her bir argümanı için
Symfony bu argümana atanacak değeri barındıran route parametresine bakar.
Yukarıdaki İleri düzey örneklerde aşağıda sıralanan değişenlerin her
hangi bir kombinasyonu (herhangi bir sıralamada)  ``showAction()`` metodunda
argüman olarak kullanılabilir:

* ``$culture``
* ``$year``
* ``$title``
* ``$_format``
* ``$_controller``

``$_controller`` değişkeni, yertutucular ve ``defaults`` kolleksiyonunun 
birleştirilmesinden dolayı vardır. Bu konudaki daha fazla açıklama için
:ref:`route-parameters-controller-arguments` belgesine bakınız.

.. tip::

    Ayrıca eşleşen route 'un adını düzenleyebilmek için özel ``$_route``
    değişkenini de kullanabilirsniz.

.. index::
   single: Routing; route kaynaklarını içeri aktarmak (import)

.. _routing-include-external-resources:

Dış Kaynaklı Route'ları içeri aktarmak
---------------------------------------

Tüm route'lar -genellikle ``app/config/routing.yml`` dosyası - tek bir
konfigürasyon dosyasından yüklenirler (bkz yukarıdaki `Route Yaratmak`_ başlığı).
Genel olarak route bilgileri bir bundle içerisinde bulunan bir route dosyası gibi
başka bir yerden yüklemek isterseniz, bu dosyayı "import" ifadesini 
kullanarak alabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"));

        return $collection;

.. note::

   YAML dosyasından kaynakları içeri alırken kullandığınız anahtarın 
   (örn: ``acme_hello``) adının hiç bir anlamı yoktur. Sadece bu adın
   diğer anahtarlar ile benzememesi gereklidir.

``resource``  anahtarı verilen route kaynağını yükler. Bu örnekte
``@AcmeHelloBundle`` kısa yolu ifadesi bu bundle'ın bulunduğu tam yeri
ifade edecek şekilde kaynağın tam yolu verilmiştir. İçeri aktarılan
(import) dosya şu şekilde olabilir:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/HelloBundle/Resources/config/routing.yml
       acme_hello:
            pattern:  /hello/{name}
            defaults: { _controller: AcmeHelloBundle:Hello:index }

    .. code-block:: xml

        <!-- src/Acme/HelloBundle/Resources/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="acme_hello" pattern="/hello/{name}">
                <default key="_controller">AcmeHelloBundle:Hello:index</default>
            </route>
        </routes>

    .. code-block:: php

        // src/Acme/HelloBundle/Resources/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('acme_hello', new Route('/hello/{name}', array(
            '_controller' => 'AcmeHelloBundle:Hello:index',
        )));

        return $collection;

Bu dosya daki route bilgileri ynı yolla ana route dosyasına yorumlanır
ve yüklenir.

İçeri Aktarılan Route'larda Ön Ek (prefix) kullanımı
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

İçeri aktarılan route bilgileri için bir "ön ek (prefix)" sağlanmasını da
seçebilirsiniz. Örneğin varsayalım `acme_hello``  route'unda basitçe 
``/hello/{name}`` yerine ``/admin/hello/{name}`` şablonunu kullanmak
istiyorsunuz:
 
.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        acme_hello:
            resource: "@AcmeHelloBundle/Resources/config/routing.yml"
            prefix:   /admin

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@AcmeHelloBundle/Resources/config/routing.xml" prefix="/admin" />
        </routes>

    .. code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $collection->addCollection($loader->import("@AcmeHelloBundle/Resources/config/routing.php"), '/admin');

        return $collection;

``/admin``  stringi artık yeni yüklenen route kaynağındaki tüm şablonların
önüne otomatik olarak gelecektir.

.. index::
   single: Routing; Hata Ayıklama

Route'ları Sanallaştırmak & Hatalarnı Ayıklamak
-----------------------------------------------
Bir route eklendiğinde ve özelleştirildiğinde route'lar hakkında bilgi almak
ve onları sanallaştırmak faydalıdır. Uygulamanız üzerindeki tüm route'ları
görmenin en iy yolu ``router:debug`` konsol komutunu çalıştırmaktır.
Projenizin kök'ünde aşağıdaki komutu çalıştırarak bunu yapabilirsiniz. 

.. code-block:: bash

    php app/console router:debug

Komut uygulamanızdaki tüm route'ların bilgisini içeren yardımcı bir listeyi
ekrana yazacaktır.

.. code-block:: text

    homepage              ANY       /
    contact               GET       /contact
    contact_process       POST      /contact
    article_show          ANY       /articles/{culture}/{year}/{title}.{_format}
    blog                  ANY       /blog/{page}
    blog_show             ANY       /blog/{slug}

Ayrıca tek bir route için oldukça özel bilgiler almak istiyorsanız komuttan
route ismini ekleyin:

.. code-block:: bash

    php app/console router:debug article_show

.. index::
   single: Routing; URL'leri Yaratmak

URL'leri Yaratmak
-----------------
Routing sistemi aynı zamanda URL'leri de yaratmada kullanılır.
Gerçekte route, controller+parametreleri ve route+parametreleri eşleştirip
URL'ye geri gönderen iyi yönlü (bi-directional) bir sistemdir.
:method:`Symfony\\Component\\Routing\\Router::match` ve
:method:`Symfony\\Component\\Routing\\Router::generate` metodları bu iki 
yönlü sistemi oluşturur. Önceki ``blog_show`` örneğinin route'una bakalım::

    $params = $router->match('/blog/my-blog-post');
    // array('slug' => 'my-blog-post', '_controller' => 'AcmeBlogBundle:Blog:show')

    $uri = $router->generate('blog_show', array('slug' => 'my-blog-post'));
    // /blog/my-blog-post

URL yararmak için route'un adını belirtmeye ve bu route için kullanılan herhangi
bir joker'e ihtiyacınız var (Örn: ``slug = my-blog-post``). Bu bilgilerle
herhangi bir URL kolaylıkla yaratılabilir:

.. code-block:: php

    class MainController extends Controller
    {
        public function showAction($slug)
        {
          // ...

          $url = $this->get('router')->generate('blog_show', array('slug' => 'my-blog-post'));
        }
    }

Yaklaşan bölümlerde bu URL'lerin şablon içerisinde nasıl yaratıldıklarını
göreceksiniz.

.. tip::

    Eğer uygulamanızın ön tarafı AJAX istekleri kullanıyorsa routing konfigürasyonuna göre
    route'ların Javascript içerisinde yaratılmasını isteyebilirsiniz. Bunun tam olarak 
    `FOSJsRoutingBundle`_ bundle'ı yapar:
    
    .. code-block:: javascript
    
        var url = Routing.generate('blog_show', { "slug": 'my-blog-post'});

    Daha fazla bilgi için bu bundle'ın dökümanlarına bakın.

.. index::
   single: Routing; Absolute URLs

Kesin URL'ler Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~
Varsayılan olarak router, göreceli(relative) URL'ler yaratır (örn:``/blog``).
Kesin URL'ler Yaratmak için ``generate()`` metodunun üçüncü argümanını 
``true`` yapmanız yeterlidir:

.. code-block:: php

    $router->generate('blog_show', array('slug' => 'my-blog-post'), true);
    // http://www.example.com/blog/my-blog-post

.. note::

    Mutlak URL'lerin yaratıldığı host geçerli ``Request`` nesnesinin
    hostudur. Bu sunucu bilgileri temel alınarak otomatik olarak PHP tarafından
    tespit edilir.Kesin URL'lerin komut satırında yaratan scriptler için
    istenen hostu ``Request`` nesnesi üzerinde manuel olarak tanımlamanız
    gereklidir:
    
    .. code-block:: php
    
        $request->headers->set('HOST', 'www.example.com');

.. index::
   single: Routing; URL'leri bir şablon içerisinden yaratmak

URL'leri Sorgu Stringleri (Query Strings) ile yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
``generate`` metodu URI yaratmak için joker değerlerini karşılayan bir arrayı 
alır ve işler. Fakat eğer eksra bir şeyler aktarmak istersniz bunlar URI'ye sorgu
stringi olarak (query string) aktarılır::

    $router->generate('blog', array('page' => 2, 'category' => 'Symfony'));
    // /blog/2?category=Symfony

URL'leri bir şablon içerisinden yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Uygulamanızda sayfalar arası linklerin yaratılmasında en sık kullanılan
yer şablonlardır. Bunu daha önceden biliyorsunuz ancak şimdi bunun için
bir şablon yardımcı (helper) fonksiyonu kullanacağız:

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ path('blog_show', { 'slug': 'my-blog-post' }) }}">
          Bu Blog Girdisini Oku.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post')) ?>">
            Bu Blog Girdisini Oku.
        </a>

Ayrıca Mutlak URL' adresleri de yaratılabilir.

.. configuration-block::

    .. code-block:: html+jinja

        <a href="{{ url('blog_show', { 'slug': 'my-blog-post' }) }}">
          Bu Blog Girdisini Oku.
        </a>

    .. code-block:: php

        <a href="<?php echo $view['router']->generate('blog_show', array('slug' => 'my-blog-post'), true) ?>">
            Bu Blog Girdisini Oku.
        </a>

Özet
----
Routing, gelen isteğin URL'si ile istek anında çalıştırılacak olan 
controller ile eşleyen sistemdir. Bunların ikiside size, spesifik güzel
URL'ler tanımlamanızı ve bu düzeltilmiş temiz URL'ler ile uygulamanızın 
fonksiyonelliğini arttırma imkanı verir.
Routing, URL'leri yaratmada kullanılan çift taraflı bir mekanizmadır.

Tarif Kitabından Daha Fazlasını Öğrenin
---------------------------------------

* :doc:`/cookbook/routing/scheme`

.. _`FOSJsRoutingBundle`: https://github.com/FriendsOfSymfony/FOSJsRoutingBundle
