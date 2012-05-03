Symfony2 Düz PHP'ye Karşı
========================
** Neden Symfony2 sadece bir dosya açıp düz bir şekilde yazdığımız PHP dosyasından
daha iyidir? **

Eğer hiç PHP framework kullanmadıysanız MVC felsefesine yabancısınızdır 
ya da Symfony2 hakkındaki hurafelere inanıyorsunuzdur. Bu bölüm sizin için.
Size Symfony2'nin düz PHP kodlamasından daha iyi bir yazılım olduğunu *söylemek*
yerine bunu kendiniz göreceksiniz.

Bu kısımda düz PHP kullarak bir uygulama yazacaksınız daha sonra onu daha
düzenli bir hale getirmek için yeniden düzenleyeceksınız.Web geliştirmenin 
son bir kaç yıl içerisinde evrim geçirerek nasıl bu hale geldiği konusunda 
bir zaman yolculuğu yapacaksınız.

ve Sonunda göreceksiniz Symfony2 nasıl sizi dünyevi sorunlardan kopartıp
kodunuzun kontrolünü size veriyor.

Düz PHP'de Basit Bir Blog
-------------------------
Bu bölümde Düz PHP kullanarak basit bir blog uygulaması geliştireceksiniz.
Başlangıçta veritabanında kayıtlı olan girdileri ekranda göstern bir sayfa
yaratın. Bunun PHP'de hızlı ve kirli bir şekilde yazılması şu şekilde:

.. code-block:: html+php

    <?php
    // index.php

    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);
    ?>

    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php while ($row = mysql_fetch_assoc($result)): ?>
                <li>
                    <a href="/show.php?id=<?php echo $row['id'] ?>">
                        <?php echo $row['title'] ?>
                    </a>
                </li>
                <?php endwhile; ?>
            </ul>
        </body>
    </html>

    <?php
    mysql_close($link);

Çabucak yazılan bu kod hızlı çalışır ancak uygulamanız büyünce bakımı 
imkansızlaşır. Ortaya bazı sorunlar çıkar:

* **Hata Denetimi Yok**: Veri tabanı bağlantısı koparsa ne olacak?

* **Zayıf organizasyon**: Eğer uygulama büyürse bu tek dosya da 
  bakım yapılamaz hale gelecektir. Form gönderilerinin kodlarını nerede 
  tutmalısınız?. Veriyi nasıl kontrol edeceksiniz. E-posta gönderen kodu
  nereye yazacaksınız ?
* ** Kodu yeniden kullanımın zorluğu**: Herşey tek dosya olduğunda 
  blogun diğer sayfalarının uygulama kodunun herhangi bir kısmının 
  kullanmasının imkanı kalmaz.

.. note::
    
    Burada değinilmeyen bir başka konuda veritabanının MySQL bağlantısıdır.
    Burada ele alınmamasına rağmen Symfony2, veritabanı ayırma ve mapping
    işlemleri için kullanılan `Doctrine`_ kütüphanesi ile tam uyumludur.
     

Bu ve diğer sorunları çözmek için çalışmaya başlayalım.

Sunumu İzole Etmek
~~~~~~~~~~~~~~~~~~~~~~~~~~

Kod, HTML "sunumunu" yapan kod ile uygulama "mantıksal" katmanından 
derhal ayrılabilmelidir:

.. code-block:: html+php

    <?php
    // index.php

    $link = mysql_connect('localhost', 'myuser', 'mypassword');
    mysql_select_db('blog_db', $link);

    $result = mysql_query('SELECT id, title FROM post', $link);

    $posts = array();
    while ($row = mysql_fetch_assoc($result)) {
        $posts[] = $row;
    }

    mysql_close($link);

    // include the HTML presentation code
    require 'templates/list.php';


HTML kodu şimdi ayrı bir (``templates/list.php``) adındaki PHP yazımındaki 
şablonvari bir dosyada tutulmaktadır:

.. code-block:: html+php

    <html>
        <head>
            <title>List of Posts</title>
        </head>
        <body>
            <h1>List of Posts</h1>
            <ul>
                <?php foreach ($posts as $post): ?>
                <li>
                    <a href="/read?id=<?php echo $post['id'] ?>">
                        <?php echo $post['title'] ?>
                    </a>
                </li>
                <?php endforeach; ?>
            </ul>
        </body>
    </html>


Kural gereği, bu dosya controller adı verin tüm uygulama mantıksal 
katmanını - ``index.php`` - barındırmaktadır. :term:`controller` terimini
framework ya da programlama dili ayırmaksızın çok fazla duyacaksınız. 
Bu basitçe kullanıcının girdilerini işleyip bir çıktı yaratan kodunuzun 
bir parçasını ifade eder.
Bu durumda controller'ımız veriyi veri tabanından hazırlar ve şablona bu
veriyi verir.Sadece şablon dosyasını değiştirerek,blog girdilerinizin 
farklı şekillerde gösterilmesini sağlayabilmeniz için 
(Örn: JSON format için ``list.json.php``) Controller şablondan ayrılmıştır.
(İzole edilmiştir.)


Uygulama (Domain) Mantıksal'ının Ayrılması (İzolasyonu)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Şimdiye kadar uygulama sadece bir sayfadan oluşuyor. Fakat eğer aynı
veritanabanı bağlantısına ihtiyacı olan ya da blog girdilerini tutan aynı
dize değişkenine ihtiyacı olan ikinci bir sayfa olursa ?
Kodu ``model.php`` adıyla uygulamadan ayrılan temel veri erişimi ve davranışları 
yapan fonksiyonların olduğu yeni bir dosya ile yeniden düzenleriz: 

.. code-block:: html+php

    <?php
    // model.php

    function open_database_connection()
    {
        $link = mysql_connect('localhost', 'myuser', 'mypassword');
        mysql_select_db('blog_db', $link);

        return $link;
    }

    function close_database_connection($link)
    {
        mysql_close($link);
    }

    function get_all_posts()
    {
        $link = open_database_connection();

        $result = mysql_query('SELECT id, title FROM post', $link);
        $posts = array();
        while ($row = mysql_fetch_assoc($result)) {
            $posts[] = $row;
        }
        close_database_connection($link);

        return $posts;
    }

.. tip::

   ``model.php`` dosya adı, uygulamanın veri erişimini sağlayan katman
   geleneksel olarak "model" katmanı olarak anıldığı için verilmiştir. 
   İyi düzenlenmiş bir uygulamada ana kod olan sizin "iş yapma mantığınız"
   (business logic) model içerisinde olmalıdır. (eğer bir kontroller
   içindeyse). Ve bu uygulamanın aksine modelin sadece bir kısmı 
   (ya da hiç birisi) gerçekten veri tabanı erişimi ile ilgilenir.

Controller (``index.php``) şimdi daha basit:

.. code-block:: html+php

    <?php
    require_once 'model.php';

    $posts = get_all_posts();

    require 'templates/list.php';


Şimdi controller içerisindeki tek görev uygulamanın model katmanından 
veriyi alır ve şablonu çağırarak veriyi ekrana basar.
Bu Model-View-Controller yapısının çok basit bir örneğidir.

Görünüm Planını (Layout) Ayırmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bu noktada uygulama  farklısayfalarda herşeyin yeniden kullanımı 
sunan ve çeşitli fırsatlar sağlayan üç ayrı parçaya bölünerek yeniden 
düzenlenmiştir.

Sadece kodun bir kısmı sayfa planında *kullanılamaz*. Bunu
yeni bir ``layout.php`` dosyası yararak düzeltiyoruz:

.. code-block:: html+php

    <!-- templates/layout.php -->
    <html>
        <head>
            <title><?php echo $title ?></title>
        </head>
        <body>
            <?php echo $content ?>
        </body>
    </html>

Şablon (``templates/list.php``) şimdi plandan (layout) daha rahat
"genişllemiştir":

.. code-block:: html+php

    <?php $title = 'List of Posts' ?>

    <?php ob_start() ?>
        <h1>List of Posts</h1>
        <ul>
            <?php foreach ($posts as $post): ?>
            <li>
                <a href="/read?id=<?php echo $post['id'] ?>">
                    <?php echo $post['title'] ?>
                </a>
            </li>
            <?php endforeach; ?>
        </ul>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>


Şimdi planın yeniden kullanımına imkan veren metodolojiye giriş yaptık.
Ne yazıkki bunu gerçekleştirmek için şablon içerisinde birkaç sinir
bozucu PHP fonksiyonunu (``ob_start()`` ve ``ob_get_clean()``) kullanmanız
gerekli. Symfony2 ``Templating`` bileşenini kullanarak bu işi size temiz
ve kolay olarak gerçekleştirir. Uygulamada ne kadar kısa olduğunu göreceksiniz.

Blog'a "show" Sayfası Eklemek
------------------------------

Blog "list" sayfası daha iyi organize edilmiş ve yeniden kullanılabilir olarak
yeniden düzenlenmiştir. ``id`` query parametresi ile belirlenen birbirleri
ile bağımsız olan blog postlarını göstermek için blog'a "show" sayfası 
eklemek gereklidir.
Başlamak için ``model.php`` dosyasının içerisinde verilen id 'ye göre
bağımsız olarak blog girdilerini getirecek yeni bir fonksiyon yaratıyoruz::

    // model.php
    function get_post_by_id($id)
    {
        $link = open_database_connection();

        $id = mysql_real_escape_string($id);
        $query = 'SELECT date, title, body FROM post WHERE id = '.$id;
        $result = mysql_query($query);
        $row = mysql_fetch_assoc($result);

        close_database_connection($link);

        return $row;
    }
Sonra, `show.php`` adında bu yeni sayfanın controller'i olan dosyayı 
yaratıyoruz:


.. code-block:: html+php

    <?php
    require_once 'model.php';

    $post = get_post_by_id($_GET['id']);

    require 'templates/show.php';

Son olarak, ``templates/show.php`` adında bağımsız blog girdilerini
ekrana basacak olan şablon dosyasını oluşturuyoruz.

.. code-block:: html+php

    <?php $title = $post['title'] ?>

    <?php ob_start() ?>
        <h1><?php echo $post['title'] ?></h1>

        <div class="date"><?php echo $post['date'] ?></div>
        <div class="body">
            <?php echo $post['body'] ?>
        </div>
    <?php $content = ob_get_clean() ?>

    <?php include 'layout.php' ?>


İkinci sayfanın yaratımı şimdi çok daha kolay oldu ve hiç bir kod tekrar
yazılmadı. Ancak hala framework'un sizin için çözebileceği bir çok sorun
var. Örneğin yanlış ya da olmayan ``id`` query parametresi verildiğinde
sayfa çokecektir. Bu durumda 404 sayfasının çıkartılması en iyi çözüm 
olacaktır ancak henüz bunu yapmak o kadar kolay değil.  Daha kötüsü 
``id`` query parametresini ``mysql_real_escape_string()`` fonksiyonu ile
SQL injection attaklarına karşı savunmayı unuttuğunuz zaman olacaktır.

Bir diğer ana sorun ise her bağımsız controller dosyuasının mutlaka
``model.php`` dosyasını kendi içerisinde çağırması zorunluluğudur.Peki
aniden diğer global süreçlerin gerçekleştirileceği (Örn: güvenliğin 
sağlanması) ek bi dosyayı çağırmak zorunda kalınırsa ? O zaman oturacaksınız
tüm controller dosyalarını tek tek açıp bunu eklemeniz gerekecek.
Eğer bir dosya içerisinde bunu eklemeyi unutursanız galiba bu dosya içeriği
güvenlikten yoksun kalacak...

Kurtarma için bir "Front Controller"
------------------------------------

Çözüm *tüm* istekleri işleyecek bir :term:`front controller` PHP dosyası 
kullanmak. Front controller ile uygulamanın URI'leri biraz değişebilir 
ancak bu daha fazla esnek hale getirir:

.. code-block:: text

    front controller olmadan
    /index.php          => Blog postları liste sayfası (index.php çalışacak)
    /show.php           => Blog postları gösterme sayfası(show.php çalışacak)

    index.php ,front controller gibi kullanılırsa
    /index.php          => Blog postları liste sayfası (index.php çalışacak)
    /index.php/show     => Blog postları gösterme sayfası(index.php çalışacak)

.. tip::
    URI'den ``index.php`` kısmı Apache rewrite kuralları (yada benzeri)
    kullanılırsa silinebilir. Bu durumda blog show sayfasının URI'sinin 
    sonucu basitçe  ``/show`` olur.
    
Tek bir PHP dosyası front controller olarak kullanıldığında (bu durumda
``index.php``) tüm *istekler* ekrana basılacaktır. Blog postları için 
show sayfası ``/index.php/show`` gerçekte ``index.php`` dosyasını çalıştıracak
ve tam URI için isteklerin yönlendirilmesinin tamamından sorumlu olacaktır.
Gördüğünüz üzere br front controller oldukça güçlü bir yardımcı araçtır.

Front Controller Yaratmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Bu uygulama ile **büyük** bir adım atmak üzeresiniz. Bir dosya ile tüm 
isteklerin işlenmesi ile güvenlik, konfigürasyon ve yönlendirme gibi 
pek çok şeyi merkezileştirebilirsiniz. Bu uygulamada ``index.php`` 
blog postlarını liste *ya da* post'u tek gösterme işini URI üzerinden
ayıracak kadar akıllı olmalıdır:


.. code-block:: html+php

    <?php
    // index.php

    // load and initialize any global libraries
    require_once 'model.php';
    require_once 'controllers.php';

    // route the request internally
    $uri = $_SERVER['REQUEST_URI'];
    if ($uri == '/index.php') {
        list_action();
    } elseif ($uri == '/index.php/show' && isset($_GET['id'])) {
        show_action($_GET['id']);
    } else {
        header('Status: 404 Not Found');
        echo '<html><body><h1>Page Not Found</h1></body></html>';
    }

Düzenli olması açısından iki controller'da (``index.php`` ve``show.php``)
şimdi ``controllers.php`` adındaki ayrı bir dosyada birer PHP fonksiyonu
olmuşlardır.

.. code-block:: php

    function list_action()
    {
        $posts = get_all_posts();
        require 'templates/list.php';
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        require 'templates/show.php';
    }

Eğer ``index.php`` front controller olarak çekirdek kütüphanelerin
yüklenmesi ve uygulamanın yönlentirme işlemleri gibi yeni bir görev aldığında
bu iki controller (``list_action()`` and ``show_action()`` fonksiyonlaeı)
çağırılacaktır. Gerçekte bu front controller mekanizması görünüm ve hareket 
olarak Symfony2'nin route'ları ve istekleri işleme mekanızmasına çok benzemeye
başlıyor.

.. tip::

   Front controller'in diğer bir avantajı esnel URL'lerdir. 
   Another advantage of a front controller is flexible URLs. Notice that
   the URL to the blog post show page could be changed from ``/show`` to ``/read``
   by changing code in only one location. Before, an entire file needed to
   be renamed. In Symfony2, URLs are even more flexible.

By now, the application has evolved from a single PHP file into a structure
that is organized and allows for code reuse. You should be happier, but far
from satisfied. For example, the "routing" system is fickle, and wouldn't
recognize that the list page (``/index.php``) should be accessible also via ``/``
(if Apache rewrite rules were added). Also, instead of developing the blog,
a lot of time is being spent working on the "architecture" of the code (e.g.
routing, calling controllers, templates, etc.). More time will need to be
spent to handle form submissions, input validation, logging and security.
Why should you have to reinvent solutions to all these routine problems?

Add a Touch of Symfony2
~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 to the rescue. Before actually using Symfony2, you need to make
sure PHP knows how to find the Symfony2 classes. This is accomplished via
an autoloader that Symfony provides. An autoloader is a tool that makes it
possible to start using PHP classes without explicitly including the file
containing the class.

First, `download symfony`_ and place it into a ``vendor/symfony/`` directory.
Next, create an ``app/bootstrap.php`` file. Use it to ``require`` the two
files in the application and to configure the autoloader:

.. code-block:: html+php

    <?php
    // bootstrap.php
    require_once 'model.php';
    require_once 'controllers.php';
    require_once 'vendor/symfony/src/Symfony/Component/ClassLoader/UniversalClassLoader.php';

    $loader = new Symfony\Component\ClassLoader\UniversalClassLoader();
    $loader->registerNamespaces(array(
        'Symfony' => __DIR__.'/../vendor/symfony/src',
    ));

    $loader->register();

This tells the autoloader where the ``Symfony`` classes are. With this, you
can start using Symfony classes without using the ``require`` statement for
the files that contain them.

Core to Symfony's philosophy is the idea that an application's main job is
to interpret each request and return a response. To this end, Symfony2 provides
both a :class:`Symfony\\Component\\HttpFoundation\\Request` and a
:class:`Symfony\\Component\\HttpFoundation\\Response` class. These classes are
object-oriented representations of the raw HTTP request being processed and
the HTTP response being returned. Use them to improve the blog:

.. code-block:: html+php

    <?php
    // index.php
    require_once 'app/bootstrap.php';

    use Symfony\Component\HttpFoundation\Request;
    use Symfony\Component\HttpFoundation\Response;

    $request = Request::createFromGlobals();

    $uri = $request->getPathInfo();
    if ($uri == '/') {
        $response = list_action();
    } elseif ($uri == '/show' && $request->query->has('id')) {
        $response = show_action($request->query->get('id'));
    } else {
        $html = '<html><body><h1>Page Not Found</h1></body></html>';
        $response = new Response($html, 404);
    }

    // echo the headers and send the response
    $response->send();

The controllers are now responsible for returning a ``Response`` object.
To make this easier, you can add a new ``render_template()`` function, which,
incidentally, acts quite a bit like the Symfony2 templating engine:

.. code-block:: php

    // controllers.php
    use Symfony\Component\HttpFoundation\Response;

    function list_action()
    {
        $posts = get_all_posts();
        $html = render_template('templates/list.php', array('posts' => $posts));

        return new Response($html);
    }

    function show_action($id)
    {
        $post = get_post_by_id($id);
        $html = render_template('templates/show.php', array('post' => $post));

        return new Response($html);
    }

    // helper function to render templates
    function render_template($path, array $args)
    {
        extract($args);
        ob_start();
        require $path;
        $html = ob_get_clean();

        return $html;
    }

By bringing in a small part of Symfony2, the application is more flexible and
reliable. The ``Request`` provides a dependable way to access information
about the HTTP request. Specifically, the ``getPathInfo()`` method returns
a cleaned URI (always returning ``/show`` and never ``/index.php/show``).
So, even if the user goes to ``/index.php/show``, the application is intelligent
enough to route the request through ``show_action()``.

The ``Response`` object gives flexibility when constructing the HTTP response,
allowing HTTP headers and content to be added via an object-oriented interface.
And while the responses in this application are simple, this flexibility
will pay dividends as your application grows.

The Sample Application in Symfony2
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The blog has come a *long* way, but it still contains a lot of code for such
a simple application. Along the way, we've also invented a simple routing
system and a method using ``ob_start()`` and ``ob_get_clean()`` to render
templates. If, for some reason, you needed to continue building this "framework"
from scratch, you could at least use Symfony's standalone `Routing`_ and
`Templating`_ components, which already solve these problems.

Instead of re-solving common problems, you can let Symfony2 take care of
them for you. Here's the same sample application, now built in Symfony2:

.. code-block:: html+php

    <?php
    // src/Acme/BlogBundle/Controller/BlogController.php

    namespace Acme\BlogBundle\Controller;
    use Symfony\Bundle\FrameworkBundle\Controller\Controller;

    class BlogController extends Controller
    {
        public function listAction()
        {
            $posts = $this->get('doctrine')->getEntityManager()
                ->createQuery('SELECT p FROM AcmeBlogBundle:Post p')
                ->execute();

            return $this->render('AcmeBlogBundle:Blog:list.html.php', array('posts' => $posts));
        }

        public function showAction($id)
        {
            $post = $this->get('doctrine')
                ->getEntityManager()
                ->getRepository('AcmeBlogBundle:Post')
                ->find($id);
            
            if (!$post) {
                // cause the 404 page not found to be displayed
                throw $this->createNotFoundException();
            }

            return $this->render('AcmeBlogBundle:Blog:show.html.php', array('post' => $post));
        }
    }

The two controllers are still lightweight. Each uses the Doctrine ORM library
to retrieve objects from the database and the ``Templating`` component to
render a template and return a ``Response`` object. The list template is
now quite a bit simpler:

.. code-block:: html+php

    <!-- src/Acme/BlogBundle/Resources/views/Blog/list.html.php --> 
    <?php $view->extend('::layout.html.php') ?>

    <?php $view['slots']->set('title', 'List of Posts') ?>

    <h1>List of Posts</h1>
    <ul>
        <?php foreach ($posts as $post): ?>
        <li>
            <a href="<?php echo $view['router']->generate('blog_show', array('id' => $post->getId())) ?>">
                <?php echo $post->getTitle() ?>
            </a>
        </li>
        <?php endforeach; ?>
    </ul>

The layout is nearly identical:

.. code-block:: html+php

    <!-- app/Resources/views/layout.html.php -->
    <html>
        <head>
            <title><?php echo $view['slots']->output('title', 'Default title') ?></title>
        </head>
        <body>
            <?php echo $view['slots']->output('_content') ?>
        </body>
    </html>

.. note::

    We'll leave the show template as an exercise, as it should be trivial to
    create based on the list template.

When Symfony2's engine (called the ``Kernel``) boots up, it needs a map so
that it knows which controllers to execute based on the request information.
A routing configuration map provides this information in a readable format:

.. code-block:: yaml

    # app/config/routing.yml
    blog_list:
        pattern:  /blog
        defaults: { _controller: AcmeBlogBundle:Blog:list }

    blog_show:
        pattern:  /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

Now that Symfony2 is handling all the mundane tasks, the front controller
is dead simple. And since it does so little, you'll never have to touch
it once it's created (and if you use a Symfony2 distribution, you won't
even need to create it!):

.. code-block:: html+php

    <?php
    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();

The front controller's only job is to initialize Symfony2's engine (``Kernel``)
and pass it a ``Request`` object to handle. Symfony2's core then uses the
routing map to determine which controller to call. Just like before, the
controller method is responsible for returning the final ``Response`` object.
There's really not much else to it.

For a visual representation of how Symfony2 handles each request, see the
:ref:`request flow diagram<request-flow-figure>`.

Where Symfony2 Delivers
~~~~~~~~~~~~~~~~~~~~~~~

In the upcoming chapters, you'll learn more about how each piece of Symfony
works and the recommended organization of a project. For now, let's see how
migrating the blog from flat PHP to Symfony2 has improved life:

* Your application now has **clear and consistently organized code** (though
  Symfony doesn't force you into this). This promotes **reusability** and
  allows for new developers to be productive in your project more quickly.

* 100% of the code you write is for *your* application. You **don't need
  to develop or maintain low-level utilities** such as :ref:`autoloading<autoloading-introduction-sidebar>`,
  :doc:`routing</book/routing>`, or rendering :doc:`controllers</book/controller>`.

* Symfony2 gives you **access to open source tools** such as Doctrine and the
  Templating, Security, Form, Validation and Translation components (to name
  a few).

* The application now enjoys **fully-flexible URLs** thanks to the ``Routing``
  component.

* Symfony2's HTTP-centric architecture gives you access to powerful tools
  such as **HTTP caching** powered by **Symfony2's internal HTTP cache** or
  more powerful tools such as `Varnish`_. This is covered in a later chapter
  all about :doc:`caching</book/http_cache>`.

And perhaps best of all, by using Symfony2, you now have access to a whole
set of **high-quality open source tools developed by the Symfony2 community**!
A good selection of Symfony2 community tools can be found on `KnpBundles.com`_.

Better templates
----------------

If you choose to use it, Symfony2 comes standard with a templating engine
called `Twig`_ that makes templates faster to write and easier to read.
It means that the sample application could contain even less code! Take,
for example, the list template written in Twig:

.. code-block:: html+jinja

    {# src/Acme/BlogBundle/Resources/views/Blog/list.html.twig #}

    {% extends "::layout.html.twig" %}
    {% block title %}List of Posts{% endblock %}

    {% block body %}
        <h1>List of Posts</h1>
        <ul>
            {% for post in posts %}
            <li>
                <a href="{{ path('blog_show', { 'id': post.id }) }}">
                    {{ post.title }}
                </a>
            </li>
            {% endfor %}
        </ul>
    {% endblock %}

The corresponding ``layout.html.twig`` template is also easier to write:

.. code-block:: html+jinja

    {# app/Resources/views/layout.html.twig #}

    <html>
        <head>
            <title>{% block title %}Default title{% endblock %}</title>
        </head>
        <body>
            {% block body %}{% endblock %}
        </body>
    </html>

Twig is well-supported in Symfony2. And while PHP templates will always
be supported in Symfony2, we'll continue to discuss the many advantages of
Twig. For more information, see the :doc:`templating chapter</book/templating>`.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`download symfony`: http://symfony.com/download
.. _`Routing`: https://github.com/symfony/Routing
.. _`Templating`: https://github.com/symfony/Templating
.. _`KnpBundles.com`: http://knpbundles.com/
.. _`Twig`: http://twig.sensiolabs.org
.. _`Varnish`: http://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
