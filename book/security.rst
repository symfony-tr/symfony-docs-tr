.. index::
   single: Güvenlik

Güvenlik
========

Güvenlik, amacı kullanıcının erişimi olmayan kaynaklara erişmesini engellemek olan 
iki aşamalı bir süreçtir.

Birinci aşamada güvenlik sistemi kullanıcının kim olduğunu belirlemek amacıyla 
br dizi tanımlamaya ihtiyaç duymasıdır. Adına **kimlik doğrulama** 
(authentication) denen bu sistem, sizin kim olduğunuzu bulmaya çalışır.

Sistem sizin kim olduğunuzu öğrendikten sonraki aşama verilen kaynağa erişim
yetkinizin olup olmadığını belirlemektir. **Yetkilendirme** (authorization) 
adı verilen bu ikinci süreç, belirli bir aksiyonu gerçekleştirmek için
yetkinizi kontrol eder.

.. image:: /images/book/security_authentication_authorization.png
   :align: center

Öğrenmenin en iyi yolu bir örnek görmek olduğundan hadi hemen başlayalım.


.. note::

    Symfony'in `güvenlik bileşeni`_ herhangi bir PHP projesinde kullanabileceğiniz
    kendi başına çalışan bir PHP kütüphanesidir.

Basit Bir Örnek : HTTP Kimlik Doğrulaması
------------------------------------------

Güvenlik bileşeni uygulama konfigürasyonunuz üzerinden ayarlanabilir.
Aslında çoğu standart güvenlik kurulumunun asıl sorunu doğru konfigürasyonu
kullanmaları durumudur. Aşağıdaki konfigürasyon Symfony'ye ``/admin/*`` 
ile eşleşen tüm URL adreslerini güenlik altına almasını ve 
kullanıcı haklarını belirlemek için basit HTTP kimlik doğrulaması kullanmasını
söyler (Örn: eski tip kullanıcı Adı / Parola kutusu):


.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    pattern:    ^/
                    anonymous: ~
                    http_basic:
                        realm: "Secured Demo Area"

            access_control:
                - { path: ^/admin, roles: ROLE_ADMIN }

            providers:
                in_memory:
                    users:
                        ryan:  { password: ryanpass, roles: 'ROLE_USER' }
                        admin: { password: kitten, roles: 'ROLE_ADMIN' }

            encoders:
                Symfony\Component\Security\Core\User\User: plaintext

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- app/config/security.xml -->

            <config>
                <firewall name="secured_area" pattern="^/">
                    <anonymous />
                    <http-basic realm="Secured Demo Area" />
                </firewall>

                <access-control>
                    <rule path="^/admin" role="ROLE_ADMIN" />
                </access-control>

                <provider name="in_memory">
                    <user name="ryan" password="ryanpass" roles="ROLE_USER" />
                    <user name="admin" password="kitten" roles="ROLE_ADMIN" />
                </provider>

                <encoder class="Symfony\Component\Security\Core\User\User" algorithm="plaintext" />
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'pattern' => '^/',
                    'anonymous' => array(),
                    'http_basic' => array(
                        'realm' => 'Secured Demo Area',
                    ),
                ),
            ),
            'access_control' => array(
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
            'providers' => array(
                'in_memory' => array(
                    'users' => array(
                        'ryan' => array('password' => 'ryanpass', 'roles' => 'ROLE_USER'),
                        'admin' => array('password' => 'kitten', 'roles' => 'ROLE_ADMIN'),
                    ),
                ),
            ),
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => 'plaintext',
            ),
        ));

.. tip::

    Standart Symfony dağıtımı güvenlik konfigürasyonunu aytı bir dosyada
    tutarak ayırır (Örn:  ``app/config/security.yml``). Eğer ayrı bir
    güvenlik dosyası istemiyorsanız, konfigürasyonu direkt ana konfigürasyon
    dosyasına koyabilirsiniz(Örn:  ``app/config/config.yml``).

Bu konfigürasyonun sonunda tam fonksiyonel bir güvenlik sistemi şu şekilde
gözükecektir:

* Sistemde iki kullanıcı vardır (``ryan`` ve ``admin``);
* Kullanıcılar kendilerini basit HTTP kimlik doğrulama ile yetkilendirmelerini yaparlar;
* ``/admin/*`` şeklinde eşleen herhangi bir URL güvenlik altına alınır ve 
  sadece ``admin`` kullanıcısı buna erişebilir;
* ``/admin/*`` ile *eşleşmeyen* herhangi bir URL tüm kullanıcılar tarafından
  erişilebilir (kullanıcılara giriş bilgisi asla sorulmayacaktır).

Şimdi kısaca güvenlik nasıl çalışıyor ve konfigürasyonun her parçası bu sürece
nasıl dahil oluyor bakalım.

Güvenlik Nasıl Çalışır: Kimlik Doğrulama ve Yetkilendirme 
---------------------------------------------------------

Symfony'nin güvenlik sistemi, kullanıcının kim olduğunu belirler ve 
sonra bu kullanıcının erişebileceği belirli kaynak ya da URL adreslerini 
konrol ederek çalışır.

Güvenlik Duvarları (Authentication)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kullanıcı güvenlik duvarı ile korunan bir URL'ye istek yaptığı zaman
güvenlik sistemi devreye girer. Güvenlik duvarının görevi kullanıcının
kimlik doğrulamaya ihtiyacı olup olmadığını belirlemek ve eğer öyleyse
kullanıcının kimlik doğrulama sürecini başlatacak cevabı göndermektir.

Bir güvenlik duvarı gelen isteğin URL'si ile güvenlik duvarının konfigürasyonunda
düzenli ifadeler (regular expression) ile belirlenen ``şablon`` (pattern)
eşleştiğinde aktif olur. Bu örnekte ``şablon`` (pattern) (``^/``) gelen
*her* istek ile eşleşir. Aslında her URL için HTTP kimlik doğrulama kullanıcı
adı ve parola ekranının gözükmesi güvenlik duvarının devrede olduğu anlamına 
gelmez. Örneğin herhangi bir kullancı ``/foo`` kaynağına kimlik doğrulama
olmadan da kolayca erişebilir.

.. image:: /images/book/security_anonymous_user_access.png
   :align: center

Güvenlik duvarı *anonim kullanıcı* ları ``anonymous`` konfigürasyon
parametresi ile kabul ettiği için bu ilk olarak çalışır. Diğer bir ifade ile
güvenlik duvarı kullanıcının doğrudan yetkili olmasına ihtiyaç duymaz. ``/foo``
kaynağına ulaşmak için özel bir ``role`` tanımı olmadığından (``access_control``
kısmı altında tanımlanan) istek, kullanıcı kimlik doğrulaması olmadan da
gerçekleştirilir.

Eğer ``anonymous`` anahtarı silinirse güvenlik duvarı *daima* kullanıcının
tam yetkili olmasını isteyecektir.

Erişim Kontrolleri (Authorization)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Eğer kullanıcı ``/admin/foo`` isteğini yaparsa , süreç farklı davranır.
Bunun nedeni ``access_control`` adlı konfigürasyon kısmında verilen 
``^/admin`` düzenli ifadesi ile eşleşen her URL'nin 
(Örn:  ``/admin`` ya da ``/admin/*`` ile eşleşen herhangibir URL) 
``ROLE_ADMIN`` rolüne sahip olması gerektiğini söylediğinden dolayıdır. 
Roller erişim kontrollerinin temelidir. Bir kullanıcı ``/admin/foo`` 
adresine sadece ``ROLE_ADMIN`` rolüne sahipse erişebilir.


.. image:: /images/book/security_anonymous_user_denied_authorization.png
   :align: center

Önceki gibi kullanıcı kendisi bir istek yaptığında güvenlik duvarı tanımlama
için bir şey sormaz. Ancak erişim katmanına ulaşır ulaşmaz kullanıcı erişimi
kısıtlanır (çünkü anonim kullanıcı ``ROLE_ADMIN`` rolüne sahip değildir),
güvenlik duvarı kullanıcı tanımlama sürecinin başladığı aksiyona zıplar.
Kimlik denetleme süreci kullandığınız kullanıcı yetkilendirme mekanizmasına
bağlıdır. Örneğin eğer form login kimlik belirleme metodu kullanıyorsanız kullanıcı
Kullanıcı Giriş sayfasına yönlenecektir. Eğer HTTP kimlik doğrulaması kullanıyorsanız
kullanıcı bir HTTP 401 mesajı gönderdiği için kullanıcı , kullanıcı adı ve parola
kutusunu görecektir.

Kullanıcı şimdi uygulamaya kimlik bilgilerini gönderme fırsatına sahiptir. Eğer
kimlik bilgileri doğru ise orijinal istek yeniden denenebilecektir.

.. image:: /images/book/security_ryan_no_role_admin_access.png
   :align: center

Bu örnekte ``ryan`` kullanıcısının güvenlik duvarı tarafından başarıyla kimliği
doğrulanır (authenticate). Fakat ``ryan`` , ``ROLE_ADMIN`` rolüne sahip
olmadığından dolayı hala ``admin/foo`` adresine erişimi engellidir. Nihayetinde
bu kullanıcı erişimin kısıtlandığını içeren bir dizi mesaj görecektir.

.. tip::

    Symfony kullanıcı erişimini kısıtladığında kullanıcı bir hata mesaj ekranı
    görecek ve bir 403 HTTP durum kodu alacaktır(``Forbidden`` (Yasaklandı)).
    Erişim engellendi ekranını :ref:`Hata Sayfaları<cookbook-error-pages-by-status-code>`
    başlıklı tarif kitabı girdisinde bulunan yönergelere göre 403 hata sayfasını
    özeleştirebilirsiniz.

Son olarak, eğer ``admin`` kullanıcısı ``/admin/foo`` adresine istekte bulunursa
ynı süreç işleyecek yalnız kimlik doğrulandıktan sonra erişim kontrol katmanı
bu isteğe izin verecektir:

.. image:: /images/book/security_admin_role_access.png
   :align: center

İstek akışı kullanıcı korunan kaynağa istekte bulunduğunda  açık fakat
inanılmaz esnektir. Daha sonra göreceğiniz üzere kullanıcı kimlik doğrulama
(authentication) form login, X.509 sertifikası ya da kullanıcıyı Twitter 
üzerinden tanımlamak gibi pek çok yolla yapılabilir. Kimlik doğrulama metodu
ne olursa olsun istek akışı daima aynıdır:

#. Bir kullanıcı korunan bir kaynağa erişir;
#. Uygulama  kullanıcıyı login formuna yönlendirir;
#. Kullanıcı kimlik bilgilerini girer (Örn:  username/password);
#. Güvenlik duvarı kullanıcının kimlik bilgilerini doğrular;
#. Kimlik bilgileri doğrulanan kullanıcı için orijinal istek tekrarlanır.

.. note::

    Süreç *aslında* biraz hangi kimlik doğrulama mekanizması kullandığınıza
    bağlıdır. Örneğin kullanıcı, form login kullandığında kullanıcının kimlik bilgileri
    kimlik bilgilerini denetleyen bir URL 'ye gönderilir (Örn:  ``/login_check``)
    ve orijinal istek adresine geri yönlendirilir(Örn:  ``/admin/foo``).
    Fakat HTTP kimlik doğrulama yöntemi kullanıldığında kullanıcı kimlik
    bilgilerini direkt orijinal URL 'ye gönderir (Örn:  ``/admin/foo``) ve
    daha sonra sayfa kullanıcının istek yaptığı aynı yere döner(no redirect).

    Bu kimlik doğrulama mekanizmalarının bu karakteristik özellikleri 
    herhangi bir probleme yol açmaz ancak bunları bu şekliyle akılda 
    tutmakta fayda vardır.
    
.. tip::

    Daha sonra Symfony2 'de belirli controller'ların, nesnelerin hatta PHP 
    metodları gibi *herhangi bir şeyin* nasıl güvenlik altına alınacağını
    öğreneceksiniz.

.. _book-security-form-login:

Geleneksel Login Formu Kullanmak
--------------------------------

.. tip::

    Bu kısımda basit bir login formunun nasıl yaratıldığını öğrenecek, devamında
    kullanıcıların detaylı bir şekilde tanımlandığı ``security.yml`` dosyasını
    kullanacaksınız.
    
    Kullanıcıları veritabanından yüklemek için lütfen :doc:`/cookbook/security/entity_provider`
    belgesini okuyun. Bu kısımı ve bu yazıyı okuyarak veritabanından kullanıcıların
    çağırıldığı tam bir login sistemi yaratabilirsiniz.

Şimdiye kadar uygulamanızın güvenlik duvarı ile nasıl örteceğinizi ve
daha sonra da roller ile belirli alanlarda nasıl koruma altına alacağınızı
gördünüz. HTTP kimlik doğrulaması ile uğraşmadan bilgilerinizi tüm tarayıcılar
tarafından sunulan doğal kullanıcı adı / parola kutusuna girebilirsiniz. Ancak
Symfony bunlardan çok daha başka kimlik doğrulama mekanizmalarına da destek verir.
Bunların hepsi için :doc:`Güvenlik Konfigürasyon Belgesi'ne</reference/configuration/security>`
bakın.

Bu kısımda, bu süreci geleneksel HTML login formu ile kullanıcı kimlik doğrulaması
yaparak geliştireceksiniz.

Öncelikle form login'i güvenlik duvarı altında aktif hale getirelim:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    pattern:    ^/
                    anonymous: ~
                    form_login:
                        login_path:  /login
                        check_path:  /login_check

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8"?>

        <srv:container xmlns="http://symfony.com/schema/dic/security"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xmlns:srv="http://symfony.com/schema/dic/services"
            xsi:schemaLocation="http://symfony.com/schema/dic/services http://symfony.com/schema/dic/services/services-1.0.xsd">

            <!-- app/config/security.xml -->

            <config>
                <firewall name="secured_area" pattern="^/">
                    <anonymous />
                    <form-login login_path="/login" check_path="/login_check" />
                </firewall>
            </config>
        </srv:container>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    'pattern' => '^/',
                    'anonymous' => array(),
                    'form_login' => array(
                        'login_path' => '/login',
                        'check_path' => '/login_check',
                    ),
                ),
            ),
        ));

.. tip::

    Eğer ``login_path`` ya da  ``check_path`` değerlerini (bu değerler
    burada varsayılan halleri ile kullanılmıştır) değiştirmeye
    ihtiyacınız yoksa konfigürasyonunuzu şu şekilde kısaltabilirsiniz:

    .. configuration-block::

        .. code-block:: yaml

            form_login: ~

        .. code-block:: xml

            <form-login />

        .. code-block:: php

            'form_login' => array(),

Şimdi güvenlik sistemi kimlik doğrulama sürecini başlattığında kullanıcı 
login formuna yönlenecektir (varsayılan olarak ``/login``). Bu login
formunu görsel olarak yapmak sizin işiniz. Öncelikle iki route yaratmalısınız.
Bir tanesi login formu gösterecek route (Örn:  ``/login``) ve diğeride
login form verisini işleyecek olan route (Örn:  ``/login_check``):

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        login:
            pattern:   /login
            defaults:  { _controller: AcmeSecurityBundle:Security:login }
        login_check:
            pattern:   /login_check

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="login" pattern="/login">
                <default key="_controller">AcmeSecurityBundle:Security:login</default>
            </route>
            <route id="login_check" pattern="/login_check" />

        </routes>

    ..  code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('login', new Route('/login', array(
            '_controller' => 'AcmeDemoBundle:Security:login',
        )));
        $collection->add('login_check', new Route('/login_check', array()));

        return $collection;

.. note::

    Güvenlik duvarı otomatik olarak bu işlemi bu URL üzerinden otomatik 
    olarak yakalayıp işleyeceğinden ``/login_check`` URL 'si için bir 
    controller yapmanıza gerek yok. Aşağıdaki form login şablonu ile isteğe 
    bağlı, fakat faydalı, olarak kullanıcı bilgilerinin girileceği 
    ekranı aşağıdaki gibi yapabilirsiniz.

``login`` route'unun adının önemli olmadığına dikkat edin. Burada önemli
olan şey, route URL'sinin (``/login``) ``login_path`` adındaki kullanıcıların
güvenlik sistemi tarafından gerektiğinde giriş yapmaları için yönlendirileceği 
konfigürasyondaki değeri ile eşleşmesidir. 


Sonra login formunu gösterecek olan controller'i yaratın:

.. code-block:: php

    // src/Acme/SecurityBundle/Controller/SecurityController.php;
    namespace Acme\SecurityBundle\Controller;

    use Symfony\Bundle\FrameworkBundle\Controller\Controller;
    use Symfony\Component\Security\Core\SecurityContext;

    class SecurityController extends Controller
    {
        public function loginAction()
        {
            $request = $this->getRequest();
            $session = $request->getSession();

            // eğer bir hata var ise
            if ($request->attributes->has(SecurityContext::AUTHENTICATION_ERROR)) {
                $error = $request->attributes->get(SecurityContext::AUTHENTICATION_ERROR);
            } else {
                $error = $session->get(SecurityContext::AUTHENTICATION_ERROR);
                $session->remove(SecurityContext::AUTHENTICATION_ERROR);
            }

            return $this->render('AcmeSecurityBundle:Security:login.html.twig', array(
                // kullanıcının girdiği son kullanıcı adı
                'last_username' => $session->get(SecurityContext::LAST_USERNAME),
                'error'         => $error,
            ));
        }
    }

Bu controller'in kafanızı karıştırmasına izin vermeyin. Şu anda göreceğiniz 
üzere kullanıcı form verisi gönderdiğinde (submit) güvenlik sistemi, form
verisi gönderimini (submission) sizin için otomatik olarak işler. 
Eğer kullanıcı geçersiz kullanıcı adı ya da parola bilgisi gönderdiyse
bu controller güvenlik sisteminden form verisi gönderme hatasını okuyarak
bunu kullanıcıya gösterir.

Diğer bir ifade ile sizin işiniz login formunu göstermek ve login hataları
oluştuğunda bunları göstermektir. Güvenlik sisteminin kendisi verilen 
kullanıcı adı ve parolaya dikkat eder ve kullanıcıyı tanımlar.

Son olarak ilgili şablonu yaratın:

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/SecurityBundle/Resources/views/Security/login.html.twig #}
        {% if error %}
            <div>{{ error.message }}</div>
        {% endif %}

        <form action="{{ path('login_check') }}" method="post">
            <label for="username">Kullanıcı Adı:</label>
            <input type="text" id="username" name="_username" value="{{ last_username }}" />

            <label for="password">Parola:</label>
            <input type="password" id="password" name="_password" />

            {#
                Eğer kullanıcının başarılı olduğunda gideceği URL'yi control etmek istiyorsanız (daha fazla detay aşağıda)
                <input type="hidden" name="_target_path" value="/account" />
            #}

            <button type="submit">Giriş Yap</button>
        </form>

    .. code-block:: html+php

        <?php // src/Acme/SecurityBundle/Resources/views/Security/login.html.php ?>
        <?php if ($error): ?>
            <div><?php echo $error->getMessage() ?></div>
        <?php endif; ?>

        <form action="<?php echo $view['router']->generate('login_check') ?>" method="post">
            <label for="username">Kullanıcı Adı:</label>
            <input type="text" id="username" name="_username" value="<?php echo $last_username ?>" />

            <label for="password">Parola:</label>
            <input type="password" id="password" name="_password" />

            <!--
                Eğer kullanıcının başarılı olduğunda gideceği URL'yi control etmek istiyorsanız (daha fazla detay aşağıda)
                <input type="hidden" name="_target_path" value="/account" />
            -->

            <button type="submit">login</button>
        </form>

.. tip::

    ``error`` değişkeni :class:`Symfony\\Component\\Security\\Core\\Exception\\AuthenticationException`
    sınıfının bir örneği olarak şablona aktarılır. Bu sınıf kimlik doğrulama hatası durumunda
    daha fazla bilgi - ya da daha hassas bilgi - içerir. Oldukça akıllıca!

Form çok az şeye ihtiyaç duyar. Birincisi, güvenlik sistemi form 
gönderisini otomatik olarak yakalayıp işleyecek ``/login_check`` 'e gönderilecek 
form. İkincisi güvenlik sisteminin ihtiyaç duyduğu ``_username`` ve ``_password``
şeklinde adlandırılan form alanları (bu alanlar :ref:`konfigüre edilebilir<reference-security-firewall-form-login>`).

Hepsi bu kadar. Form verisini gönderdiğinizde güvenlik sistemi otomatik olarak
kullanıcı kimlik bilgilerini kontrol ederen kullanıcıyı doğrulayacak ya da 
bir hata oluştuğunda kullanıcıyı hataları da gösterebilecek şekilde login
formuna geri gönderecektir.

Tüm süreci değerlendirelim:

#. Kullanıcı korumalı alana ulaşmayı dener;
#. Güvenlik duvarı kimlik doğrulama sürecini kullanıcıyı login formuna
   (``/login``) yönlendirerek başlatır.
#. ``/login`` sayfası login formunu bu örnekte yaptığımız controller ve
   route aracılığı ile ekrana basar;
#. Kullanıcı login formundan verileri ``/login_check`` 'e gönderir;
#. Güvenlik sistemi isteği yakalar, kullanıcı kimlik bilgilerini kontrol eder,
   eğer kullanıcı doğru ise kimlik bilgilerini doğrular ve eğer kullanıcı
   doğrulanmadıysa logn formuna geri gönderilir.

Varsayılan olarak eğer gönderilen kimlik bilgileri doğru ise kullanıcı 
istek yapılan orijinal sayfaya yönlendirilecektir (Örn:  ``/admin/foo``).
Eğer kullanıcı kendisi direkt login sayfasına giderse, kullanıcı 
ana sayfaya yönlendirilecektir. Bu istenildiği kadar, sizin izin 
verdiğiniz ölçüde, örneğin belirlenen bir URL adresine yönlendirme gibi,
geniş bir şekilde özelleştirilebilir.

Bu konuda hakkında daha fazla detay ve login sürecini genel olarak
nasıl düzenleyebileceğiniz konusunda daha fazla bilgi için 
:doc:`/cookbook/security/form_login` belgesine bakın.

.. _book-security-common-pitfalls:

.. sidebar:: Tuzaklardan Kaçınmak

    Login formu yaparken şu genel tuzaklara dikkat edin.

    **1. Doğru route'ları yaratın.**

    Öncelikle ``/login`` ve ``/login_check`` routelarını düzgün bir şekilde
    ve ilgili ``login_path`` ve ``check_path`` konfigürasyon değerlerini 
    de doğru bir şekilde tanımladığınıza emin olun. Yanlış yapılan
    bir konfigüasyon login sayfası yerine 404 sayfasına yönlenecektir ya da
    login formunun gönderilen form verisi hiç bir şey yapmayacaktır (login
    formu tekrar tekrar gözükecektir).

    **2. login sayfasının korumalı olmadığına emin olun**

    Ayrıca login sayfasının herhangi bir rol gereksinimi olmadan görüntülenebildiğine
    emin olun. Örneğin aşağıdaki konfigürasyon tüm URL'ler için (``/login`` URL'side dahil)
    ``ROLE_ADMIN`` rolu ne gereksinim duyacak bir yeniden yönlendirme döngüsüne 
    girecektir:

    .. configuration-block::

        .. code-block:: yaml

            access_control:
                - { path: ^/, roles: ROLE_ADMIN }

        .. code-block:: xml

            <access-control>
                <rule path="^/" role="ROLE_ADMIN" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/', 'role' => 'ROLE_ADMIN'),
            ),

    
    ``/login`` URL'sini erişim kontrolünden sildiğinizde sorun çözülür:

    .. configuration-block::

        .. code-block:: yaml

            access_control:
                - { path: ^/login, roles: IS_AUTHENTICATED_ANONYMOUSLY }
                - { path: ^/, roles: ROLE_ADMIN }

        .. code-block:: xml

            <access-control>
                <rule path="^/login" role="IS_AUTHENTICATED_ANONYMOUSLY" />
                <rule path="^/" role="ROLE_ADMIN" />
            </access-control>

        .. code-block:: php

            'access_control' => array(
                array('path' => '^/login', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY'),
                array('path' => '^/', 'role' => 'ROLE_ADMIN'),
            ),

	Ayrıca eğer güvenlik duvarı anonim kullanıcılara izin *vermiyorsa* ,
	özel bi güvenlik duvarı yaratarak bu kullanıcıların login sayfasına
	erişimine izin verdirebilirsiniz:
	
    .. configuration-block::

        .. code-block:: yaml

            firewalls:
                login_firewall:
                    pattern:    ^/login$
                    anonymous:  ~
                secured_area:
                    pattern:    ^/
                    form_login: ~

        .. code-block:: xml

            <firewall name="login_firewall" pattern="^/login$">
                <anonymous />
            </firewall>
            <firewall name="secured_area" pattern="^/">
                <form_login />
            </firewall>

        .. code-block:: php

            'firewalls' => array(
                'login_firewall' => array(
                    'pattern' => '^/login$',
                    'anonymous' => array(),
                ),
                'secured_area' => array(
                    'pattern' => '^/',
                    'form_login' => array(),
                ),
            ),

    **3. ``/login_check`` 'in bir güvenlik duvarı arkasında olduğuna emin olun**

	``check_path`` URL değerinin (Örn:  ``/login_check``) login formu için
	kullandığınız güvenlik duvarı arkasında olduğuna emin olun (bu örnekte
	tek güvenlik duvarı ``/login_check`` de dahil *tüm* URL'leri kapsar).
	Eğer ``/login_check`` bir güvenlik duvarı tarafından eşleşmezse 
	``Unable to find the controller for path "/login_check"`` hatasını 
	alırsınız.
	
    **4. Çoklu güvenlik duvarları, güvenlik içeriğini paylaşmaz**

    Eğer çoklu güvenlik duvarı kullanıyor ve kimlik doğrulamayı bir 
    güvenlik duvarında yapıyorsanız diğer güvenlik duvarları tarafından
    otomatik olarak kimliğiniz *doğrulanmaz*. Farklı güvenlik duvarları
    farklı güvenlik sistemleri gibidir. Bu yüzden çoğu uygulamaya 
    bir güvenlik duvarı yeterli olmaktadır.

Yetkilendirme (Authorization)
-----------------------------

Güvenliğin ilk adımı daima kullanıcının kim olduğunun doğrulandığı kimlik doğrulamadır.
Symfony2 ile kimlik doğrulama form login, basit HTTP kimlik denetimi ya da 
hatta Facebook üzerinden bile herhangi bir yolla yapılabilir.

Kullanıcının bir kere kimliği doğrulandımı yetkilendirme başlar. Yetkilendirme (Authorization)
eğer kullanıcının erişebileceği herhangi bir kaynak(bir URL, bir model nesnesi, bir
metod çağrısı, ...) varsa bunun kararını vermede standart ve güçlü bir yoldur.
Bu her kullanıcıya belirli roller atayarak ve farklı kaynakların farklı rollere
gereksinim duymasıyla olur. 

Yetkilendirme iki farklı tarafta gerçekleşir:

#. Kullanıcının sahip olduğu belirli roller;
#. Bir kaynağın erişilebilir olması için ihtiyaç duyduğu rol.

Bu kısımda farklı kaynakları (URL'ler, metod çağrıları vs...) farklı rollerle 
nasıl güvenlik altına alabileceğimize bakacağız. Daha sonra rollerin nasıl yaratıldığı
ve kullancılara atandığı konusunda daha fazla şey öğreneceksiniz.

Belirli URL Şablonlarını Güvenlik Altına Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Uygulamanızı güvenlik altına almanın en temel yolu tüm URL şablonlarını 
güvenlik altına almaktır. Bunu bu kısmın en başında ``^/admin`` şablonunun
``ROLE_ADMIN`` rolü ile eşleştirildiği örnekte zaten görmüştünüz.

Pek çok URL şablonunda olduğu gibi ihtiyacınıza göre herbirisini bir düzenli
ifade ile tanımlayabilirsiniz.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/admin/users, roles: ROLE_SUPER_ADMIN }
                - { path: ^/admin, roles: ROLE_ADMIN }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <rule path="^/admin/users" role="ROLE_SUPER_ADMIN" />
            <rule path="^/admin" role="ROLE_ADMIN" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'access_control' => array(
                array('path' => '^/admin/users', 'role' => 'ROLE_SUPER_ADMIN'),
                array('path' => '^/admin', 'role' => 'ROLE_ADMIN'),
            ),
        ));

.. tip::

    Yolu  ``^`` karakteri ile başlatmak bu URL'nin sadece  *başlangıcında*
    desenin eşleşmesini sağlar. Örneğin basitçe ``/admin`` (``^`` karakteri olmadan)
    doğru bir şekilde ``/admin/foo`` eşleşebilirken aynı zamanda ``/foo/admin`` 'de
    eşleşecektir.

Her gelen istekte Symfony2 erişim kontrol kuralı içerisinde bunları eşlemeye 
çalışır(ilk bulunan kazanır). Eğer kullanıcı hala doğrulanmadıysa kimlik 
doğrulama süreci başlatılır(Kullanıcıya login olma şansı verilir). 
Ancak kullanıcı *doğrulanmış* fakat gerekli role sahip değilse 
:class:`Symfony\\Component\\Security\\Core\\Exception\\AccessDeniedException`
istisnası atılarak kullanıcıya güzel bir "Erişim Engellendi" sayfası gösterilir.
Daha fazla bilgi için :doc:`/cookbook/controller/error_pages` belgesine bakınız.

Symfony,  ``/admin/users/new`` gibi bir URL ile karşılaştığında ilk kural 
ile eşleştirecek ve sadece ``ROLE_SUPER_ADMIN`` rolüne ihtiyaç duyacaktır. 
``/admin/blog`` gibi herhangi bir URL, ``ROLE_ADMIN`` rol'üne ihtiyaç 
duyan ikinci kural ile eşleşecektir.

.. _book-security-securing-ip:

IP ile Güvenlik Sağlamak
~~~~~~~~~~~~~~~~~~~~~~~~

Erişimi route'taki IP numarası üzerinden kısıtlamak istediğiniz durumlar
olabilir. Bu kısmen :ref:`Edge Side Includes<edge-side-includes>` durumu
ile ilgilidir. Örneğin,"_internal" olarak adlandırılan route'larda.
ESI kullanıldığında _internal route'u verilen sayfanın altkısımları için farklı
ön bellekleme seçeneklerini gateway ön belleği tarafından aktif edilmesi
için gerekebilir. bu route ^/_internal ön eki ile varsayılan olarak 
standart sürüm ile birlikte gelir (routing dosyasında bu satırların 
aktif olduğu varsayılmıştır).

Aşağıda bu route'u dışarıdan erişimlere karşı güvenli hale getirebileceğiniz
bir örnek verilmiştir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/_internal, roles: IS_AUTHENTICATED_ANONYMOUSLY, ip: 127.0.0.1 }

    .. code-block:: xml

            <access-control>
                <rule path="^/_internal" role="IS_AUTHENTICATED_ANONYMOUSLY" ip="127.0.0.1" />
            </access-control>

    .. code-block:: php

            'access_control' => array(
                array('path' => '^/_internal', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY', 'ip' => '127.0.0.1'),
            ),

.. _book-security-securing-channel:

Kanal Tarafından Güvenlik 
~~~~~~~~~~~~~~~~~~~~~~~~~

IP tabanlı güvenliğe çok benzer olarak access_control başlığı altında
yapacağınız küçük bir değişiklikle SSL kullanma durumunuda 
gerçekleştirebilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            access_control:
                - { path: ^/cart/checkout, roles: IS_AUTHENTICATED_ANONYMOUSLY, requires_channel: https }

    .. code-block:: xml

            <access-control>
                <rule path="^/cart/checkout" role="IS_AUTHENTICATED_ANONYMOUSLY" requires_channel="https" />
            </access-control>

    .. code-block:: php

            'access_control' => array(
                array('path' => '^/cart/checkout', 'role' => 'IS_AUTHENTICATED_ANONYMOUSLY', 'requires_channel' => 'https'),
            ),

.. _book-security-securing-controller:

Controller'i Güvenlik Altına Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Uygulamanızı URL şablonları temelinde koruma altına almanız kolaydır ancak
belirli durumlar için yeterli olmayabilir. Gerekli olduğunda
kolaylıkla controller içerisinden kimlik doğrulamasına zorlanabilir:


.. code-block:: php

    use Symfony\Component\Security\Core\Exception\AccessDeniedException;
    // ...

    public function helloAction($name)
    {
        if (false === $this->get('security.context')->isGranted('ROLE_ADMIN')) {
            throw new AccessDeniedException();
        }

        // ...
    }

.. _book-security-securing-controller-annotations:

Ayrıca opsiyonel olarak controller'larınızı belirteçler (annotation) ile
güvenlik altına almak istiyorsanız ``JMSSecurityExtraBundle`` 'ı kurup
kullanabilirsiniz:

.. code-block:: php

    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Secure(roles="ROLE_ADMIN")
     */
    public function helloAction($name)
    {
        // ...
    }

Daha fazla bilgi için `JMSSecurityExtraBundle`_  belgesini okuyun. Eğer
Symfony Standart Sürüm kullanıyorsanız bu bundle varsayılan olarak gelmektedir.
Eğer standart sürüm kullanmıyorsanız kolaylıkla download edebilir ve kurabilirsiniz.

Diğer Servisleri Güvenlik Altına Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Aslında Symfony'de hiç bir şey önceki kısımda gördüğünüz gibi bir strateji
ile korunmaz. Örneğin varsayalım bir kullanıcıdan diğerine e-posta gönderen
bir servisiniz(Örn: bir PHP sınıfı) var. Bu sınıfın kullanımını -nereden 
kullanıldığının bir önemi yok- belirli rollerdeki kullanıcıların kullanımına
kısıtlayabilirsiniz.

Uygulamanızda security bileşeninin farklı servis ve metodları nasıl
koruduğu hakkındaki bilgi için :doc:`/cookbook/security/securing_services`
belgesine bakın.

Erişim Kontrol Listeleri (ACL'ler): Bağımsız Veritabanı Nesnelerini Güvenlik Altına Almak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Düşünün, kullanıcılarınızın yorumlar yapabildiği yeni bir blog sistemi 
tasarlıyorsubuz. Kullanıcının kendi yorumlarını düzenlemesini istiyor
ancak diğerlerininkileri düzenlemesini istemiyorsunuz. Ayrıca admin
kullanıcısının kendininkileride dahil *tüm* yorumları düzenlemesi gerekiyor.

The security component comes with an optional access control list (ACL) system
that you can use when you need to control access to individual instances
of an object in your system. *Without* ACL, you can secure your system so that
only certain users can edit blog comments in general. But *with* ACL, you
can restrict or allow access on a comment-by-comment basis.

Security bileşeni isteğe bağlı olarak bir nesnenin bağımsız örneklerini (instance)
gerektiğinde erişimini kontrol edecek bir erişim kontrol listesi (ACL) siste
mi ile birlikte gelir. ACL *olmadan genel olarak sisteminizde sadece belirli 
kullanıcıların blog yorumlarını düzenlemesini sağlayabilirsiniz. Fakat 
ACL *kullanarak* yorumlar bazında erişimi engelleyebilir ya da izin
verebilirsiniz.

Daha fazla bilgi için tarif kitabındaki :doc:`/cookbook/security/acl` bölümünü
okuyun.

Kullanıcılar
------------

Önceki bölümlerde farklı kaynakları belirlediğiniz *rol* tipleri ile 
nasıl koruyabileceğinizi öğrendiniz. bu kısımda yetkilendirmenin diğer 
tarafı olan kullanıcıları inceleyeceğiz.

Kullanıcılar Nereden Gelir (*Kullanıcı Sağlayıcıları*)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kimlik doğrulama esnasında kullanıcı bir dizi kimlik bilgisini
(genellikle kullanıcı adı ve parola) girer. Kimlik doğrulama sisteminin işi
bu kimlik bilgilerinin bir kullanıcı havuzunda eşleşip eşleşmediğini bulmaktır.
Peki bu kullanıcıların listesi nereden geliyor?

Symfony2 'de kullanıcılar bir konfigürasyon dosyasından, bir veritabanı
tablosundan, bir web servisinden yada düşünebildiğiniz herhangi bir yerden gelebilir.
Bir ya da daha fazla kullanıcıyı kullanıcı yetkilendirme sistemine sağlayan
herhangi bir şey "kullanıcı sağlayıcı" (user provider) olarak adlandırılır.
Symfony2 iki adet en sık kullanılan kullanıcı sağlayıcısı ile birlikte gelir.
bunlardan bir tanesi kullanıcıları konfigürasyon dostasından çağırır, diğeri
ise veritabanı tablosundan çağırır.

Kullanıcıları Konfigürasyon Dosyasında Belirlemek
.................................................

Aslında kullanıcıları belirlemenin en kolay yolu, direkt olarak bu 
kullanıcıları konfigürasyon dosyasında belirlemektir. Aslında bu örneği 
bu bölümde önceden görmüştünüz.

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                default_provider:
                    users:
                        ryan:  { password: ryanpass, roles: 'ROLE_USER' }
                        admin: { password: kitten, roles: 'ROLE_ADMIN' }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <provider name="default_provider">
                <user name="ryan" password="ryanpass" roles="ROLE_USER" />
                <user name="admin" password="kitten" roles="ROLE_ADMIN" />
            </provider>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'providers' => array(
                'default_provider' => array(
                    'users' => array(
                        'ryan' => array('password' => 'ryanpass', 'roles' => 'ROLE_USER'),
                        'admin' => array('password' => 'kitten', 'roles' => 'ROLE_ADMIN'),
                    ),
                ),
            ),
        ));

Bu kullanıcı sağlayıcısı kullanıcıları veritabanı içerisinde herhangi bir 
şekilde sağlamadığından "hafıza içi" (in-memory) kullanıcı sağlayıcısı
olarak adlandırılır. Gerçek kullanıcı nesnesi Symfony tarafından sağlanır
(:class:`Symfony\\Component\\Security\\Core\\User\\User`).

.. tip::

    Herhangi bir kullanıcı sağlayıcısı kullanıcıları direkt olarak 
    konfigürasyon dosyasında ``users``    konfigürasyon parametresi ile listelenen
    yerden direkt olarak çağırabilir.

.. caution::

    Eğer kullanıcı adınız tamamen sayılardan (Örn:  ``77``) ya da 
    tire içeriyorsa (Örn:  ``user-name``) bu durumda kullanıcıları YAML
    dosyası altında tanımlarken şu alternatif yazım şeklini kullanmalısınız:

    .. code-block:: yaml

        users:
            - { name: 77, password: pass, roles: 'ROLE_USER' }
            - { name: user-name, password: pass, roles: 'ROLE_USER' }

Küçük siteler için bu metod hızlı bir şekilde uygulanabilir. Daha karmaşık
sistemlerde kullanıcıları veri tabanından çağırmak isteyeceksinizdir.

.. _book-security-user-entity:

Kullanıcıları Veritabanından Çağırmak
.....................................

Eğer kullanıcılarınızı Doctrine ORM aracılığı ile çağırmak istiyorsanız
bunu ``User`` adında bir sınıf yaratıp konfigürasyonda ``entity``
anahtarı altında belirterek kolaylıkla yapabilirsiniz.

.. tip:

    Kullanıcıları Doctrine ORM ya da ODM altında saklamanıza izin veren
    yüksek kaliteli açık kaynak kodlu bir bundle da bulunmaktadır. Bunun
    hakkında daha fazla bilgi için GitHub üzerindeki `FOSUserBundle`_ 'a
    bakın.

Bu yaklaşımda öncelikle veritanabanındaki saklama işlemleri için 
kullanılacak olan ``User`` sınıfınızı yaratın.

.. code-block:: php

    // src/Acme/UserBundle/Entity/User.php
    namespace Acme\UserBundle\Entity;

    use Symfony\Component\Security\Core\User\UserInterface;
    use Doctrine\ORM\Mapping as ORM;

    /**
     * @ORM\Entity
     */
    class User implements UserInterface
    {
        /**
         * @ORM\Column(type="string", length=255)
         */
        protected $username;

        // ...
    }

Güvenlik sisteminin özel user sınıfınızda gerekli duyduğu tek şey,
kullanıcı sınıfınızın :class:`Symfony\\Component\\Security\\Core\\User\\UserInterface`
interface'i üzerinden türetilmesidir. Bunun anlamı, konsept olarak "user",
bu interface üzerinden türetildiği takdirde her şey olabilir demektir.

.. note::

    User nesnesi istek (request) halinde serileştirilip oturumda saklanacağından
    dolayı nesnenizi `\Serializable interface 'i üzerinden yapılandırmanızı`_ 
    tavsiye ederiz. Bu özellikle eğer ``User`` sınıfınızın private sınıf değişkenleri
    olan bir üst sınıfı varsa önemli olacaktır.

Sonra, ``entity`` tipinde ``User`` sınıfınızı işaret eden bir kullanıcı
sağlayıcısı konfigüre edin:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                main:
                    entity: { class: Acme\UserBundle\Entity\User, property: username }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <provider name="main">
                <entity class="Acme\UserBundle\Entity\User" property="username" />
            </provider>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'main' => array(
                    'entity' => array('class' => 'Acme\UserBundle\Entity\User', 'property' => 'username'),
                ),
            ),
        ));

Bu yeni sağlayıcının başlangıcında, kimlik doğrulama sistemi veritabanından
``User`` sınıfını bu sınıfın ``username`` alanı ile yüklemeye çalışacaktır.

.. note::
    Bu örneğin amacı sadece ``entity`` sağlayıcısının nasıl çalıştığı konusundaki
    basit fikri göstermek içindir. Tam çalışan bir örnek için 
    :doc:`/cookbook/security/entity_provider` belgesine bakın.

Kendi özel kullanıcı sağlayıcınızın nasıl yaratabileceğiniz konusunda 
(Örn: Eğer kullanıcılarınızı bir web servisi aracılığı ile yükleyecekseniz)
:doc:`/cookbook/security/custom_provider` belgesine bakın.

.. _book-security-encoding-user-password:

Kullanıcı Parolalarını Şifrelemek
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Şimdiye kadar, kolaylık olması açısından, tüm örneklerde kullanıcı parolaları
düz metinler olarak saklandı(bu kullanıcılar konfigürasyon dosyasında ya da 
veri tabanında olup olmaması önemli değil). Elbette gerçek bir uygulamada
kullanıcılarınızın parolalarını güvenlik nedeni ile şifrelemek isteyeceksiniz.
Bu eşleştirdiğiniz ve tanımladığınız Kullanıcı sınıfınız için önceden 
sağlanan hazır "encoder" 'lar aracılığı ile kolaylıkla yapılabilir. 
Örneğin kullanıcılarınızı hafıza içinde tutuyorsunuz (in-memory). 
Kullanıcı parolalarını ``sha1`` ile şu şekilde şifreleyebilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...
            providers:
                in_memory:
                    users:
                        ryan:  { password: bb87a29949f3a1ee0559f8a57357487151281386, roles: 'ROLE_USER' }
                        admin: { password: 74913f5cd5f61ec0bcfdb775414c2fb3d161b620, roles: 'ROLE_ADMIN' }

            encoders:
                Symfony\Component\Security\Core\User\User:
                    algorithm:   sha1
                    iterations: 1
                    encode_as_base64: false

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->
            <provider name="in_memory">
                <user name="ryan" password="bb87a29949f3a1ee0559f8a57357487151281386" roles="ROLE_USER" />
                <user name="admin" password="74913f5cd5f61ec0bcfdb775414c2fb3d161b620" roles="ROLE_ADMIN" />
            </provider>

            <encoder class="Symfony\Component\Security\Core\User\User" algorithm="sha1" iterations="1" encode_as_base64="false" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...
            'providers' => array(
                'in_memory' => array(
                    'users' => array(
                        'ryan' => array('password' => 'bb87a29949f3a1ee0559f8a57357487151281386', 'roles' => 'ROLE_USER'),
                        'admin' => array('password' => '74913f5cd5f61ec0bcfdb775414c2fb3d161b620', 'roles' => 'ROLE_ADMIN'),
                    ),
                ),
            ),
            'encoders' => array(
                'Symfony\Component\Security\Core\User\User' => array(
                    'algorithm'         => 'sha1',
                    'iterations'        => 1,
                    'encode_as_base64'  => false,
                ),
            ),
        ));

``iterations`` değerini ``1`` 'e ve ``encode_as_base64`` değerini false yaparak
basitçe ``sha1`` algoritmasını bir kere ve ekstra şifreleme yapmadan çalıştırdık.
Şimdi şifrelenmiş parolayı ya programsal olarak (Örn:  ``hash('sha1', 'ryanpass')``)
ya da `functions-online.com`_ gibi bir online araçla hesaplayabilirsiniz.

Eğer kullanıcıları dinamik olarak yaratıyorsanız (ve onları veri tabanında da 
saklıyorsanız), daha zor şifreleme algoritmaları kullanabilir ve gerçek 
parola şifreleme nesnenizin bu algoritmaları kullanmasını bekleyebilirsiniz. Örneğin
varsayalım kullanıcı nesneniz ``Acme\UserBundle\Entity\User`` (yukarıdaki örnekteki gibi).
Öncelikle bu kullanıcı için şifreleyiciyi (encoder) konfigüre edelim:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            # ...

            encoders:
                Acme\UserBundle\Entity\User: sha512

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <!-- ... -->

            <encoder class="Acme\UserBundle\Entity\User" algorithm="sha512" />
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            // ...

            'encoders' => array(
                'Acme\UserBundle\Entity\User' => 'sha512',
            ),
        ));

Böylece  daha güçlü ``sha512`` algoritmasını kullanmış oldunuz. Ayrıca,
algoritmayı bir metin olarak (``sha512``) belirttiğinizden dolayı, sistem
varsayılan olarak parolanızı bir satırda 5000 kez şifreleyecek ve sonra
onu base64 olarak şifreyeleyecek. Diğer bir ifade ile parola oldukça fazla
bir şekilde şifrelenerek geri açılamaz (decode) hale geldi(şifrelenmiş bir
paroladan gerçek parolayı belirleyemezsiniz).

Eğer kullanıcı kaydı (register) için bir dizi form kullanıyorsanız, kullanıcı nesneniz
içerisinde set edebileceğiniz bir şifrelenmiş parola kısmı belirlemeniz gereklidir. 
Kullanıcı nesnesinde hangi şifreleme algoritması kullandığınız önemli değildir. 
Şifrelenmiş bir parola her zaman controller içerisinde şu şekilde belirlenir:

.. code-block:: php

    $factory = $this->get('security.encoder_factory');
    $user = new Acme\UserBundle\Entity\User();

    $encoder = $factory->getEncoder($user);
    $password = $encoder->encodePassword('ryanpass', $user->getSalt());
    $user->setPassword($password);

Kullanıcı Nesnesini Almak
~~~~~~~~~~~~~~~~~~~~~~~~~

Kimlik doğrulamasından sonra geçerli kullanıcının ``User`` nesnesi ``security.context``
servisi tarafından erişilebilir. Controller içerisinden bu şu şekilde yapılır:

.. code-block:: php

    public function indexAction()
    {
        $user = $this->get('security.context')->getToken()->getUser();
    }

.. note::

    Anonim kullanıcılar ''isAuthenticated()`` metodu
    anonim kullanıcı nesnesini true olarak döndüreceği için teknik olarak
    kimlikleri doğrulanır. Bir kullanıcının kimliğinin doğrulanma 
    bilgisini almak için ``IS_AUTHENTICATED_FULLY`` rolünü kontrol
    etmelisiniz.
    
    
Twig şsblonunda bu nesneye 
:method:`GlobalVariables::getUser()<Symfony\\Bundle\\FrameworkBundle\\Templating\\GlobalVariables::getUser>`
metodunu çağıran ``app.user`` anahtarı ile ulaşılabilir:

.. configuration-block::

    .. code-block:: html+jinja

        <p>Username: {{ app.user.username }}</p>


Çoklu Kullanıcı Sağlayıcılarını Kullanmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Her kimlik doğrulama mekanizması (Örn: HTTP Kimlik doğrulaması, form login, vs...)
sadece bir kullanıcı sağlayıcısı kullanır ve varsayılan olarak ilk belirtilen
kullanıcı sağlayıcı mekanizması kullanılır. Fakat acaba birkaç kullanıcı 
konfigürasyon dosyası üzerinden, geri kalanları da veritabanından sağlanacaksa
ne olacak? Bu iki sağlayıcıyıda zincirleme olarak kullanabilen yeni
bir sağlayıcı yaratmakla mümkün olabilir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            providers:
                chain_provider:
                    providers: [in_memory, user_db]
                in_memory:
                    users:
                        foo: { password: test }
                user_db:
                    entity: { class: Acme\UserBundle\Entity\User, property: username }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <provider name="chain_provider">
                <provider>in_memory</provider>
                <provider>user_db</provider>
            </provider>
            <provider name="in_memory">
                <user name="foo" password="test" />
            </provider>
            <provider name="user_db">
                <entity class="Acme\UserBundle\Entity\User" property="username" />
            </provider>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'providers' => array(
                'chain_provider' => array(
                    'providers' => array('in_memory', 'user_db'),
                ),
                'in_memory' => array(
                    'users' => array(
                        'foo' => array('password' => 'test'),
                    ),
                ),
                'user_db' => array(
                    'entity' => array('class' => 'Acme\UserBundle\Entity\User', 'property' => 'username'),
                ),
            ),
        ));

Şimdi ilk olarak tanımlandığından dolayı tüm kimlik doğrulama mekanizmaları 
``chain_provider`` 'ı kullanacaklar. ``chain_provier`` kendi içerisinde 
kullanıcıları ``in_memory`` ve ``user_db`` altında tanımlanan 
sağlayıcıları kullanmaya çalışacaktır.

.. tip::

    Eğer kullanıcılarınızı ``in_memory`` kullanıcıları ve ``user_db``
    kullanıcıları olarak ayırmak için bir nedeniniz yoksa bu iki kaynağı
    oldukça kolay bir şekilde birleştirip, tek bir sağlayıcı olarak da 
    kullanabilirsiniz:

    .. configuration-block::

        .. code-block:: yaml

            # app/config/security.yml
            security:
                providers:
                    main_provider:
                        users:
                            foo: { password: test }
                        entity: { class: Acme\UserBundle\Entity\User, property: username }

        .. code-block:: xml

            <!-- app/config/security.xml -->
            <config>
                <provider name=="main_provider">
                    <user name="foo" password="test" />
                    <entity class="Acme\UserBundle\Entity\User" property="username" />
                </provider>
            </config>

        .. code-block:: php

            // app/config/security.php
            $container->loadFromExtension('security', array(
                'providers' => array(
                    'main_provider' => array(
                        'users' => array(
                            'foo' => array('password' => 'test'),
                        ),
                        'entity' => array('class' => 'Acme\UserBundle\Entity\User', 'property' => 'username'),
                    ),
                ),
            ));

Ayrıca güvenlik duvarını ya da bağımsız kimlik doğrulama mekanizmalarınıda
belirli bir sağlayıcı ile çalışacak şekilde konfigüre edebilirsiniz. 
Yine bir sağlayıcı belirtilmedikçe ilk sağlayıcı her zaman kullanılan olacaktır:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    # ...
                    provider: user_db
                    http_basic:
                        realm: "Secured Demo Area"
                        provider: in_memory
                    form_login: ~

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall name="secured_area" pattern="^/" provider="user_db">
                <!-- ... -->
                <http-basic realm="Secured Demo Area" provider="in_memory" />
                <form-login />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    // ...
                    'provider' => 'user_db',
                    'http_basic' => array(
                        // ...
                        'provider' => 'in_memory',
                    ),
                    'form_login' => array(),
                ),
            ),
        ));

Bu örnekte eğer kullanıcı HTTP kimlik doğrulamasıyla giriş yapmaya çalışırsa,
kimlik doğrulama sistemi ``in_memory`` kullanıcı sağlayıcısını kullanacaktır.
Ancak eğer kullanıcı login işlemini form login üzerinden yaparsa ``user_db``
kullanıcı sağlayıcısı kullanılacaktır(tüm güvenlik duvarının tamamında
varsayılan olmasına karşın).

Kullanıcı sağlayıcısı ve güvenlik duvarı konfigürasyonu hakkında daha
fazla bilgi için :doc:`/reference/configuration/security` belgesine
bakın.

Roller
------

"role" 'ün arkasındaki fikir yetkilendirme sisteminin anahtarı olmasıdır.
Her kullanıcıya bir dizi rol atanır ve her kaynak bir ya da daha fazla
role gereksinim duyar. Eğer kullanıcı gerekli olan rollere sahipse,
erişim sağlanır. Aksi takdirde erişim yasaklanır.

Roller oldukça basit ve gerektiğinde kendi kendinize uydurabileceğiniz
basit metinlerdir(roller içsel olarak nesne olmalarına rağmen). Örneğin
eğer sitenizin blog admin kısmının erişimini kısıtlamak istiyorsanız
bu kısmı bir ``ROLE_BLOG_ADMIN`` rolü ile koruma altına alabilirsiniz.
Bu rol herhangi bir yerde tanımlamaya ihtiyaç duymaz, bu rolü
hemen kullanarak başlayabilirsiniz.

.. note::

    Tüm roller Symfony2 tarafından yönetilmesi için ``ROLE_`` ön ekine
    sahip **olmalıdır**. Eğer kendi rollerinizi özel ``Role`` sınıfı ile
    tanımladıysanız (çok ileri düzey) ``ROLE_`` ön ekini kullanmayın. 

Hiyerarşik Roller
~~~~~~~~~~~~~~~~~

Pek çok rolü kullanıcılara bağlamak yerine bir rol hiyerarşisi yaratarak
rol kalıtım kurallarını belirleyebilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            role_hierarchy:
                ROLE_ADMIN:       ROLE_USER
                ROLE_SUPER_ADMIN: [ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <role id="ROLE_ADMIN">ROLE_USER</role>
            <role id="ROLE_SUPER_ADMIN">ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH</role>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'role_hierarchy' => array(
                'ROLE_ADMIN'       => 'ROLE_USER',
                'ROLE_SUPER_ADMIN' => array('ROLE_ADMIN', 'ROLE_ALLOWED_TO_SWITCH'),
            ),
        ));

Yukarıdaki konfigürasyonda ``ROLE_ADMIN`` rolune sahip olan kullanıcılar ``ROLE_USER``
rolüne sahip olacaklardır. ``ROLE_SUPER_ADMIN`` rolü ``ROLE_ADMIN``, ``ROLE_ALLOWED_TO_SWITCH``
ve ``ROLE_USER`` rolüne sahiptir (``ROLE_ADMIN`` 'den kalıtım yoluyla)

Güvenli Çıkış (Logging Out)
---------------------------

Genellikle kullanıcıların güvenli çıkış yapmasını sağlamak isteyeceksiniz.
Çok şükür ki güvenlik duvarı bunu ``logout`` konfigürasyon parametresi
devrede ise otomatik olarak gerçekleştirebilir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                secured_area:
                    # ...
                    logout:
                        path:   /logout
                        target: /
            # ...

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall name="secured_area" pattern="^/">
                <!-- ... -->
                <logout path="/logout" target="/" />
            </firewall>
            <!-- ... -->
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'secured_area' => array(
                    // ...
                    'logout' => array('path' => 'logout', 'target' => '/'),
                ),
            ),
            // ...
        ));

Bu güvenlik duvarı altında bir kere ayarlandımı kullanıcıyı ``/logout`` 'a 
gönderildiğinde (ya da ``path`` ne olarak konfigüre edildiyse) geçerli
kullanıcı'nın oturumu kapatılacaktır. Kullanıcı ana sayfaya yönlendirilecektir.
(``target`` parametresinin değerinde ne yazıyorsa). ``path`` ve ``target`` 
konfigürasyon parametrelerinden ikiside varsayılan değeri olarak sizin belirlediğiniz
değerleri alır. Diğer bir ifade ile siz bunları düzenlemedikçe bu parametreleri
koymayarak konfigürasyonunuzu kısaltabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        logout: ~

    .. code-block:: xml

        <logout />

    .. code-block:: php

        'logout' => array(),

Güvenlik duvarının tümünü gerçekleştirdiği için ``/logout`` URL'si için bir controller
geliştirmeye gerek duymayacağınıza dikkat edin. Ancak belki bu URL'yi yaratacak bir
route yaratabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/routing.yml
        logout:
            pattern:   /logout

    .. code-block:: xml

        <!-- app/config/routing.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>

        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing http://symfony.com/schema/routing/routing-1.0.xsd">

            <route id="logout" pattern="/logout" />

        </routes>

    ..  code-block:: php

        // app/config/routing.php
        use Symfony\Component\Routing\RouteCollection;
        use Symfony\Component\Routing\Route;

        $collection = new RouteCollection();
        $collection->add('logout', new Route('/logout', array()));

        return $collection;

Kullanıcı oturumunu kapattığımda yukarıda ``target`` parametresinde belirlenen
(Örn:  ``homepage``) yere yönlendirilecektir. sistemden çıkış işlemi (logout)
için daha fazla bilgi öğrenmek için 
:doc:`Güvenlik Konfigürasyon Belgesine Bakın</reference/configuration/security>`.

Şablonlarda Erişim Kontrolü
---------------------------

Eğer geçerli kullanıcı bir şablon içerisinde bir rolü varsa ve eğer
bunu kontrol etmek istiyorsanız önceden hazır yardımcı fonksiyonları
kullanabilirsiniz:

.. configuration-block::

    .. code-block:: html+jinja

        {% if is_granted('ROLE_ADMIN') %}
            <a href="...">Sil</a>
        {% endif %}

    .. code-block:: html+php

        <?php if ($view['security']->isGranted('ROLE_ADMIN')): ?>
            <a href="...">Sil</a>
        <?php endif; ?>

.. note::

    Eğer bu fonksiyonu kullanırsanız ve güvenlik duvarı içerisinde aktif 
    bir URL yok ise bir istisna (exception) atılacaktır. Yine en iyi fikir 
    ana güvenlik duvarının tüm URL'leri kapsamasıdır(bu bölüm içerisinde
    gösterilmişti).

Controller İçerisinde Erişim Kontrolü
-------------------------------------

Eğer geçerli kullanıcı controller içerisinde bir role sahipse ve bunu 
kontrol etmek istiyorsanız güvenlik içeriğinin (security context) 
``isGranted`` metodunu kullanın: 

.. code-block:: php

    public function indexAction()
    {
        // admin kullanıcılarına farklı içerik göster
        if ($this->get('security.context')->isGranted('ROLE_ADMIN')) {
            // burada admin içeriğini yükle
        }
        // genel içeriği burada yükle
    }

.. note::

    Bir güvenlik duvarı'nın ``isGranted`` metodu çağırıldığında istisna
    üretmemesi için aktif olması gerekmektedir. Şablonlar hakkındaki
    yukarıda verilen nota bakarak daha fazl bilgi alabilirsiniz.

Bir Kullanıcıyı Taklit Etmek
----------------------------

Bazen bir kullanıcıdan diğer bir kullanıcıya sistemden çıkış yapmadan geçmek
faydalı olabilir(mesela hata ayıklarken ya da bir hatanın kullanıcı tarafında
nasıl gözüktüğünü anlamak için). Bu ``switch_user`` güvenlik duvarı dinleyicisini
aktif ederek oldukça kolay bir şekilde sağlanabilir:

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    # ...
                    switch_user: true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <!-- ... -->
                <switch-user />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => true
                ),
            ),
        ));

Başka bir kullanıcıya geçmek için sadece geçerli URL'nize ``_switch_user``
sorgu parametresini (query string) ekleyip bu değere bir kullanıcı adı
atamanız yeterlidir:

    http://example.com/somewhere?_switch_user=thomas

Orijinal kullanıcıya geri dönmek için ``_exit`` özel kullanıcı adını kullanın:

    http://example.com/somewhere?_switch_user=_exit

Elbette bu özellik küçük bir kullanıcı gurubunda kullanılabilir. Varsayılan olarak
erişim ``ROLE_ALLOWED_SWITCH`` rolüne sahip kullanıcılarla sınırlandılırmıştır.
Bu rolün adı ``role`` ayarı ile değiştirilebilir. Ekstra güvenlik için ayrıca
sorgu parametresini (query parameter)  düzenlemek için ``parameter`` seçeneğinin
değerini değiştirebilirsiniz: 

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    // ...
                    switch_user: { role: ROLE_ADMIN, parameter: _olmak_istenen_kullanici }

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall>
                <!-- ... -->
                <switch-user role="ROLE_ADMIN" parameter="_olmak_istenen_kullanici" />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main'=> array(
                    // ...
                    'switch_user' => array('role' => 'ROLE_ADMIN', 'parameter' => '_olmak_istenen_kullanici'),
                ),
            ),
        ));

Kayıtsız(stateless) Kimlik Doğrulama
------------------------------------

Varsayılan olarak Symfony2 kullanıcının güvenlik içeriğini bir çerezde (oturum)
saklar. Eğer sertifika kullanıyorsanız ya da HTTP kimlik doğrulaması kullanıyorsanız
bu kullanıcı bilgileri her istek için herhangi bir yerde saklanması gerekmekz. 
Bu durumda eğer istekler arasında herhangi bir şey saklamayacaksanız, kayıtsız
(stateless) kimlik doğrulamayı aktif edebilirsiniz(bu Symfony2 tarafından herhangibir
çerez oluşturulmayacak anlamına gelmektedir):

.. configuration-block::

    .. code-block:: yaml

        # app/config/security.yml
        security:
            firewalls:
                main:
                    http_basic: ~
                    stateless:  true

    .. code-block:: xml

        <!-- app/config/security.xml -->
        <config>
            <firewall stateless="true">
                <http-basic />
            </firewall>
        </config>

    .. code-block:: php

        // app/config/security.php
        $container->loadFromExtension('security', array(
            'firewalls' => array(
                'main' => array('http_basic' => array(), 'stateless' => true),
            ),
        ));

.. note::

    Eğer form login kullanıyorsanız, Symfony2 ``stateless` değerini ``true``
    yapsanız bile bir çerez yaratacaktır.

Son Sözler
----------

Güvenlik uygulamanız içerisinde doğru bir şekiklde çözülmesi gereken 
derin ve karmaşık bir konu olabilir. Şükür ki, Symfony'nin security bileşeni, 
iyice test edilmiş *kimlik doğrulama* (authentication) ve *yetkilendirme*
(authorization) etrafında şekillendirilmiş bir güvenlik modelini kullanır.
Kimlik doğrulama her zaman ilk önce ve güvenlik duvarının bu işi farklı 
metodlar kullanarak denetleyebilmesiyle olur (Örn:  HTTP kimlik doğrulaması,
login form, vs...). Tarif kitabında kimlik doğrulamayı gerçekleştiren diğer
metodları, çerez kullanarak "beni hatırla" özelliğini geliştirme gibi konuları
bulacaksınız. 

Kullanıcının kimliği doğrulandığında yetkilendirme katmanı kullanıcının 
belirli bir kaynağa erişiminin olup olmadığını belirler. Genel olarak *roller*
URL'lere, sınıflara ya da metodlara uygulanır ve eğer kullanıcı bunlara
erişme hakkına sahip değilse erişemez. Yekilendirme katmanı bu yüzden
çok derindir ve geçerli kullanıcının verilen kaynağa erişim yetkisini 
belirleyebildiği için çok parçalı bir "karar verme" (voting) sistemi kullanır.
Bu konudaki daha fazla bilgiyi tarif kitabının diğer başlıklarında araştırın.



Tarif Kitabından Daha Fazlasını Öğrenin
---------------------------------------

* :doc:`HTTP/HTTPS Zorlamak</cookbook/security/force_https>`
* :doc:`Özel bir karar verici ile kullanıcıların IP adreslerine göre kara listeye almak </cookbook/security/voters>`
* :doc:`Erişim Kontrol Listeleri(ACL) </cookbook/security/acl>`
* :doc:`/cookbook/security/remember_me`

.. _`güvenlik bileşeni`: https://github.com/symfony/Security
.. _`JMSSecurityExtraBundle`: https://github.com/schmittjoh/JMSSecurityExtraBundle
.. _`FOSUserBundle`: https://github.com/FriendsOfSymfony/FOSUserBundle
.. _`\Serializable interface 'i üzerinden yapılandırmanızı`: http://php.net/manual/en/class.serializable.php
.. _`functions-online.com`: http://www.functions-online.com/sha1.html
