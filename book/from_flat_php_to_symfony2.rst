Symfony2 Düz PHP'ye Karşı
========================
**Neden Symfony2 sadece bir dosya açıp düz bir şekilde yazdığımız PHP dosyasından
daha iyidir?**

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
* **Kodu yeniden kullanımın zorluğu**: Herşey tek dosya olduğunda 
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
yüklenmesi ve uygulamanın yönlendirme işlemleri gibi yeni bir görev aldığında
bu iki controller (``list_action()`` ve ``show_action()`` fonksiyonları)
çağırılacaktır. Gerçekte bu front controller mekanizması görünüm ve hareket 
olarak Symfony2'nin route'ları ve istekleri işleme mekanızmasına çok benzemeye
başlıyor.

.. tip::

   Front controller'in diğer bir avantajı esnek URL'lerdir.
   Önceleri bir blog girdisinin isminin öncelikle değiştirilmesi
   gerekiyordu.Blog post sayfasının URL lerini sadece bir yerden  ``/show`` dan ``/read``
   olarak değiştirebildiğini hatırlayın.Symfony2'de URL'ler oldukça esnektir.

Şimdi ise uygulama tek dosyadan kodu yeniden kullanılabilmesine olanak 
sağlayacak bir yapıya dönüştü. Mutlu olmalısınız ancak bitmedi.Örneğin
"yönlendirme" sistemi kararsız ve ``/`` dan erişirse 
bu liste sayfasını tanımayacak (``/index.php``) (Eğer Apache rewrite kuralları
eklendiyse). Ayrıca blog geliştirmek terine kodun "mimari" yapısına (Örn
yönlendirme, controller'ların çağırılması, şablonlar vs..) oldukça fazla
zaman harcadınız. Form verilerinin işlenmesi, girdilerin kontrolü, loglama
ve güvenlik işlemleri için daha da vakit harcamalısınız. Neden bu rutin 
sorunların önceden yapılmış çözümlerini kendiniz yapmaya çalışıyorsunuz ?

Bir Symfony2 Dokunuşu Ekleyin.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony2 kurtarır. Önceden gerçten Symfony2 kullanılsaydı, PHP'nin Symfony2
sınıflarını nasıl bulduğunu bilmeniz gerkecekti. Bu Symfony 'nin sağladığı 
bir otomatik yükleyici tarafından gerçekleştirilir. Bir otomatik yükleme 
aracı sınıfları içeren dosyaların başlangıçta dosyalar içerisinden 
tanımlamadan otomatik olarak yüklenmesine olanak sağlar.
Öncelikle  `symfony'i indirin`_  ve ``vendor/symfony/`` dizinine yerleştirin.
Sonra bir  ``app/bootstrap.php`` dosyası yaratın ve uygulamada autoloader'i
konfigüre eden sistemi çalıştırmak için ``require`` ile bunları çağırın:

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



Bu autoloader'a ``Symfony`` sınıflarının nerede olduğunu söyler. Bununla
siz Symfony sınıflarını ilgili dosyaların içerisinde ``require``  ifadesini
kullanarak çağırmadan kullanabilirsiniz.

Symfony'nin ana felsefesi bir uygulamanın ana işinin gelen her isteği 
yorumlamak ve bir cevap döndürmek olduğu düşüncesidir. Bunun sonucunda 
Symfony2 :class:`Symfony\\Component\\HttpFoundation\\Request`  ve  
:class:`Symfony\\Component\\HttpFoundation\\Response` adındaki iki sınıf
ile birlikte gelir. 
Bu sınıflar nesne-yönelimli olarak ham HTTP isteklerini işler ve HTTP 
cevapları döndürmeye başlar. Bunları kullanarak blogunuzu geliştirin:

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


Controllerkar şimdi ``Response`` nesnesini döndürmekten sorumludur.
Bunu daha kolak yapmak için ``render_template()`` adında tesadüfen Symfony2'nin
şablon motoru işlemlerine çok benzeyen, bir fonksiyon kullanabilirsiniz:

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

Symfony2'nin küçük bir parçasının alınıp kullanılmasıyla uygulama daha
esnek ve güvenilir bir hale geldi.
``Request`` HTTP isteğine güvenilir bir şekilde erişmek için bir yol sağlar.
Özellikle ``getPathIngo()`` metodu temizlenmiş bir URI 
(daima ``/show`` döner. Asla  ``/index.php/show`` dönmez) döndürür.
Bu yüzden eğer kullanıcı ``/index.php/show`` isteğini yapsa bile, 
uygulama zekice davranarak isteği ``show_action()`` a yönlendirir.

``Response`` nesnesi HTTP cevapları 
oluşturmada,HTTP başlıklarını kabul etmedede ve nesne-yönelimli 
bir arabirim vasıtasıyla  içerik olusturmada esneklik verir.

The ``Response`` object gives flexibility when constructing the HTTP response,
allowing HTTP headers and content to be added via an object-oriented interface.
Response'lar bu uygulamada basit iken bu esneklik uygulama büyüdükçe 
size daha fazla fayda sağlayacaktır.

Symfony2'de Örnek Uygulama
~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Blog *uzun* bir yol katetmesine rağmen hala böyle bir uygulama için çok
fazla kod geliştirilmelidir. Bu yolda biz ayrıca basit bir yönlendirme
sistemini ve şablonları render ederken ``ob_start()`` ve ``ob_get_clean()``
fonksiyonlarını keşfettik.
 
Eğer bazı nedenlerden bu "framework" 'u sıfırdan inşaa etmeye gereksinim
duyuyorsanız en azından Symfony'nin kendi başına çalışabilen ve bu 
problemleri zaten çözmüş olan `Yönlendirme`_ ve `Şablon`_ bileşenlerini
kullanabilirsiniz.

Bilinen sorunların yeniden çözümü yerine Symfony2'nin sizin yerinize bunları
nasıl çözdüğüne bakabilirsiniz. Buarada aynı uygulamanın Symfony2 ile
yapılmış hali bulunmaktadır:

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
İki controller hala basit ve az kod içeriyor. Her ikiside veritabanından
nesneleri getirmek için Doctrine ORM kütüphanesini ve ``Şablon`` bileşenini
``Response`` nesnesinden gelen içeriği ekrana basmak için kullanıyor. 
Listeleme yapan şablon ise biraz daha basit:

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

Layout ise neredeyse aynı:

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

    show şablonunu örnekte yapmayacağız çünki neredeyse yarattığımız list
    şablonu ile tamamen aynı.

Symfony2'nin motoru ilk kalkışta (``Kernel`` adını alır) hangi controller'ın
hangi istek bilgisinde çalışacağını bilmesi için haritalamaya(map) ihtiyaç duyar.
Bir yönlendirme konfigürasyonu bu bilgiyi okunabilir bir formatta sağlar:

.. code-block:: yaml

    # app/config/routing.yml
    blog_list:
        pattern:  /blog
        defaults: { _controller: AcmeBlogBundle:Blog:list }

    blog_show:
        pattern:  /blog/show/{id}
        defaults: { _controller: AcmeBlogBundle:Blog:show }

Şimdi Symfony2, tüm olağan görevleri çok basit olan bir front controller
aracılığı ile denetlemektedir. Bu o kadar az çalışırki, bir kere oluşturduktan sonra
asla dokunmak zorunda kalmazsınız. (ve eğer Symfony2 dağıtımı kullanıyorsanız
bunuda asla yaratmak zorunda değilsiniz!):

.. code-block:: html+php

    <?php
    // web/app.php
    require_once __DIR__.'/../app/bootstrap.php';
    require_once __DIR__.'/../app/AppKernel.php';

    use Symfony\Component\HttpFoundation\Request;

    $kernel = new AppKernel('prod', false);
    $kernel->handle(Request::createFromGlobals())->send();


Front controller'in görevi sadece Symfony2'nin motorunu (``Kernel``)
başlatmak ve gelen ``Request`` nesnesini kontrol etmektir.
Symfony2'nin çekirdeği yönlendirme haritası ile hangi kontroller'ın
çağrı yaptığını belirler.Önceki gibi controller metodları sadece 
enson ``Response`` nesnesini döndürmekten sorumludur. Gerçekten sadece
budur. Başka bir şey değil. 

Görsel olarak Symfony2'nin her isteğin nasıl kontrol edildiğini görmek
için :ref:`istek akış diyagramı<request-flow-figure>` 'na bakın.

Symfony2 Neler Verir
~~~~~~~~~~~~~~~~~~~~~~~

Gelecek olan bölümlerde Symfony2'nin her bir parçası hakkında ve bir 
projenin tavsiye edilen yapısı ile ilgili daha fazla şey öğreneceksiniz.
Şimdi blog'un düz PHP'den Symfony2'ye aktarım sürecinde ne gelişti bakalım:

* Uygulamanız şimdi daha **temiz ve daima organize koda** sahip (Symfony
  sizi buna zorlamamasına rağmen). Bu **yeniden kullanılabilirliği** ve
  yeni geliştiricilerin proje içerisinde daha verimli ve çabuk olmasını
  sağlar.

* Kodun 100%'ünü *uygulamanıza* yazarsınız. :ref:`autoloading<autoloading-introduction-sidebar>`,
  :doc:`routing</book/routing>`, ya da rendering :doc:`controllers</book/controller>`.
  gibi *Düşük seviye* işlemleri geliştirmeye gerek kalmaz.

* Symfony2 size Doctrine, Templating, Güvenlik, Form,
  Veri Doğrulama ve Çeviri bileşenleri (daha pek çok olan) gibi 
  **açık kaynak yardımcı araçlara erişmenize** olanak verir.

* Uygulama şimdi ``Routing`` bileşeni yardımı ile **tamamen esnek URL** yapısıyla
  daha güzeldir.

* Symfony2'nin HTTP merkezli mimarisi size **Symfony2'nin içsel HTTP cache**
  sistemi ile güçlendirilmiş **HTTP önbelleklemesi** ya da `Varnish`_ güçlü
  araçlara erişimi sağlar.
  

Ve belkide en iyisi, Symfony2 kullanırken, **Symfony2 topluluğu tarafından
geliştirilen yüksek kaliteli açık kaynak tool'lara** erişebilmektir!!
Symfony2 topluluğu tarafından geliştiren araçları bulabilmeniz için 
`KnpBundles.com`_ iyi bir seçimdir.

Daha İyi Şablonlar
------------------

Eğer kullanmayı seçerseniz Symfony2 `Twig`_ adndaki şablonları 
daha hızlı okumanızı ve yazmanızı sağlayan bir standart şablon motoru ile
birlikte gelir.
Bunun anlamı örnek uygulamanız çok daha az kod tutacaktır!! Örneğin
blog için listeleme şablonunun Twig haline bakın:

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

İlgili ``layout.html.twig`` şablonunuda yazmak çok kolay:

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

Twig Symfony2'de tam desteklenir. Twig'in pek çok avantajını konuşurken
PHP şavlonlarıda her zaman Symfony2 tarafından desteklenecektir.
Daha fazla bilgi almak için kitabın :doc:`şablon kısmına </book/templating>`
bakın.

Tarif Kitabından Daha Fazlasını Öğrenin
----------------------------------------

* :doc:`/cookbook/templating/PHP`
* :doc:`/cookbook/controller/service`

.. _`Doctrine`: http://www.doctrine-project.org
.. _`symfony'i indirin`: http://symfony.com/download
.. _`Yönlendirme`: https://github.com/symfony/Routing
.. _`Şablon`: https://github.com/symfony/Templating
.. _`KnpBundles.com`: http://knpbundles.com/
.. _`Twig`: http://twig.sensiolabs.org
.. _`Varnish`: http://www.varnish-cache.org
.. _`PHPUnit`: http://www.phpunit.de
