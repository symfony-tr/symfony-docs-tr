Controller
==============
İlk iki kısımdan sonra hala bizimle misiniz?. Symfony2 bağımlısı olmaya
başladınız!. 
Fazla uzatmadan Controller'ların neler yapabildiğine bakalım.

Formatları Kullanmak
--------------------

Günümüzde bir web uygulaması sadece HTML sayfalarından daha fazla farklı 
çıktı vermelidir.RSS yüklemeleri için XML 'e ya da Web Servisleri ve Ajax
istekleri için JSON çıkılarına pek çok seçim yapılmalıdır.  Bu formatları 
desteklemek Symfony2 'de oldukça kolaydır. Route belirtecinin defaults 
parametresinin  ``_format`` değerini ``xml`` düzenlemek yeterlidir::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}", defaults={"_format"="xml"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Request tipinin kullanımında (``_format`` değişkeninde tanımlanır) Symfony2
otomatık olarak doğru şablon tipini seçecektir. Burada örneğin ``hello.xml.twig``:

.. code-block:: xml+php

    <!-- src/Acme/DemoBundle/Resources/views/Demo/hello.xml.twig -->
    <hello>
        <name>{{ name }}</name>
    </hello>

Hepsi bukadar. Standart formatlar için Symfony2 ayrıca cevap başlığında 
``Content-Type`` otomatik olarak en iyi formatı seçecektir. 

Eğer tek action'da farklı formatlara destek vermek istiyorsanız ``{_format}``
yer tutucusunun değerini değiştirmeniz yeterlidir::

    // src/Acme/DemoBundle/Controller/DemoController.php
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;

    /**
     * @Route("/hello/{name}.{_format}", defaults={"_format"="html"}, requirements={"_format"="html|xml|json"}, name="_demo_hello")
     * @Template()
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Controller şimdi ``/demo/hello/Fabien.xml`` ya da 
``/demo/hello/Fabien.json`` şeklindeki URL adresi ile çağırılacaktır.

``requirements`` girdisi yertutucunun eşleşmesi gereken düzenli ifadeyi
tanımlar. 
Örnekte eğer ``/demo/hello/Fabien.js`` şeklinde bir istek yapmaya çalışırsanız
``_format`` 'taki tanımlama ile eşleşmediğinden 404 HTTP hatası alırsınız.

Yönlendirme (Redirecting) ve Gönderme(Forwarding)
--------------------------------------------------

Eğer bir kullanıcıyı birsayfaya yönlendirmek istiyorsanız, ``redirect()``
metodunu kullanın::

    return $this->redirect($this->generateUrl('_demo_hello', array('name' => 'Lucas')));

``generateUrl()`` metodu şablonlar içerisinde kullandığımız  ``path()`` 
fonksiyonu ile aynıdır.Bu fonksiyon route adını ve dize değişkenindeki
argümanlarıda alarak birleştirilmiş basit bir URL döndürür.

Ayrıca bir actionu ``forward()`` metodu ile başka bir action'a iletebilirsiniz.
İçerde Symfony bir "alt istek" yaparak alt isteğin ``Response`` nesnesini
çevirir ::

    $response = $this->forward('AcmeDemoBundle:Hello:fancy', array('name' => $name, 'color' => 'green'));

    // response olarak bir şeyler yap ya da direkt olarak döndür

İsteğin (Request) Bilgisini almak. 
------------------------------------

Yönlendirme yer tutucuların (route placeholder) değerlerinin yanında 
controller aynı zamanda ``Request`` nesnesine de ulaşabilir::

    $request = $this->getRequest();

    $request->isXmlHttpRequest(); // bir Ajax isteği mi?

    $request->getPreferredLanguage(array('en', 'fr'));

    $request->query->get('page'); // $_GET parametresini al

    $request->request->get('page'); // $_POST parametresini al

Şablon içerisinde de aynı zamanda ``Request`` nesnesine 
``app.request`` değişkeni ile ulaşabilirsiniz:

.. code-block:: html+jinja

    {{ app.request.query.get('page') }}

    {{ app.request.parameter('page') }}

Oturumda(Session) verinin kalıcılığını sağlamak.
------------------------------------------------

HTTP protokolü sağlamasa bile Symfony2 istemciyi temsil eden güzel bir
oturum (session) nesnesi sağlar(tarayıcıyı kullanan gerçek bir kişi olur,
bir bot olur ya da bir web serivisi olur).

İki istek arasında Symfony2 doğal (native) PHP oturumlarını kullanarak bir 
çerez ile değerleri depolar.

Oturumdan bilgiyi almak ya da saklamak herhangi bir controller'dan kolay
lıkla halledilebilir::

    $session = $this->getRequest()->getSession();

    // kullanıcının sonraki isteğinde kullancağı değeri sakla
    $session->set('foo', 'bar');

    // controller'da başka bir istek için 
    $foo = $session->get('foo');

    // kullanıcı ülkesini değiştir.
    $session->setLocale('tr');

Ayrıca hemen sonraki isteğe aktarılması için basit mesajlarda 
saklayabilirsiniz::

    // hemen sonraki isteğe aktarılacak mesaj (controller içerisinde)
    $session->setFlash('notice', 'Congratulations, your action succeeded!');

    // Sonraki açılan sayfada gösterilecek mesaj (Şablon içerisinde)
    {{ app.session.flash('notice') }}


Bu başarılı olduğunuz mesajı göstererek kullanıcıyı diğer sayfaya gönderme
işlemi gibi ir şeye ihtiyacınız olduğunda oldukça kullanışlıdır (baştan
yönlenir sonra mesaj gösterilir.)

Kaynakların Güvenliğini Sağlamak
---------------------------------


Symfony2 Standart sürümü pek çok güvenlik ihyitiyacını karşılayacak basit
bir güvenlik konfigürasyonu ile birlikte gelir:


.. code-block:: yaml

    # app/config/security.yml
    security:
        encoders:
            Symfony\Component\Security\Core\User\User: plaintext

        role_hierarchy:
            ROLE_ADMIN:       ROLE_USER
            ROLE_SUPER_ADMIN: [ROLE_USER, ROLE_ADMIN, ROLE_ALLOWED_TO_SWITCH]

        providers:
            in_memory:
                users:
                    user:  { password: userpass, roles: [ 'ROLE_USER' ] }
                    admin: { password: adminpass, roles: [ 'ROLE_ADMIN' ] }

        firewalls:
            dev:
                pattern:  ^/(_(profiler|wdt)|css|images|js)/
                security: false

            login:
                pattern:  ^/demo/secured/login$
                security: false

            secured_area:
                pattern:    ^/demo/secured/
                form_login:
                    check_path: /demo/secured/login_check
                    login_path: /demo/secured/login
                logout:
                    path:   /demo/secured/logout
                    target: /demo/


Bu konfigürasyon kullanıcıların ``user`` ve ``admin`` parametrelerine
uyan giriş (login) yapmış kullanıcıların ``/demo/secured/` ile başlayan herhangi
bir URL 'ye girmelerine ihtiyacı içindir.

Dahası ``admin`` kullanıcısının ``ROLE_ADMIN`` rolü ``ROLE_USER``  rolünüde
kapsamaktadır. (``role_hierarchy`` ayarına bakınız.)

.. tip::

    Okunabilir olması için parolalar temel konfigürasyonda basit metin
    ler olarak saklanır ancak herhangi bir karıştırıcı (hash) algoritmasını
     ``encoders`` kısmını kullanarak ta ayarlayabilirsiniz.

``http://localhost/Symfony/web/app_dev.php/demo/secured/hello`` URL'sine gittiğinizde
URL  sizi otomatik olarak login sayfasına aracaktır. Çünki bu kaynak  ``firewall`` 
tarafından korunmaktadır.

Aynı zamanda Controller'ınız da ``@Secure`` belirteci ile verdiğiniz roller
ile güçlendirebilirsiniz::

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use JMS\SecurityExtraBundle\Annotation\Secure;

    /**
     * @Route("/hello/admin/{name}", name="_demo_secured_hello_admin")
     * @Secure(roles="ROLE_ADMIN")
     * @Template()
     */
    public function helloAdminAction($name)
    {
        return array('name' => $name);
    }

Şimdi güvenli hello sayfasından ``user`` olarak giriş yapın 
(``ROLE_ADMIN`` rolüne sahip *olmadan*) ve Hello resource secured" linkine
tıklayın. Symfony2 403 HTTP durumunu döndürmüş olmalı. Bu kullanıcının 
bu kaynağa erişiminin yasaklandığını belirtmektedir.

.. note::

    Symfony2 güvenlik katmanı oldukça esnek ve pekçok kullanıcı sağlayıcısı 
    (user provider) (mesela bir tanesi Doctrine ORM gibi) ve kullanıcı
    doğrulayıcısı ile birlikte gelir.(HTTP basic, HTTP digest,
    ya da  X509 sertifikasyonu gibi) Bu konu hakkında daha fazla bilgi
    almak ve nasıl konfigüre edildiğini öğrenmek için ":doc:`/book/security`"
    bölümünü okuyun.

Kaynakların Ön Belleğe Alınması (Caching)
-----------------------------------------


Web sitenizin trafiği artar artmaz aynı kaynaklara tekrar tekrar erişme
durumundan kaçınacaksınız. Symfony2 HTTP ön bellekleme başlıklarını 
kullanarak kaynaklarınızı yönetir. Basit bir ön bellekleme stratejisi için 
``@Cache()`` belirtecini kullanın :: 

    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Route;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Cache;

    /**
     * @Route("/hello/{name}", name="_demo_hello")
     * @Template()
     * @Cache(maxage="86400")
     */
    public function helloAction($name)
    {
        return array('name' => $name);
    }

Bu örnekte kaynak bir günlüğüne önbelleğe alınmıştr. Fakat ihtiyaçlarınıza
göre bu ön bellekleme zamanını doğrulama özelliği ile birleştirip istediğiniz
gibi kullanabilirsiniz.

Kaynak önbelleklemesi Symfony2'nin ters vekil (reverse proxy) sistemi ile
yönetilir. Ancak ön bellekleme temel HTTP önbellekleme başlıkları tarafından
yönetildiğinden ön tanımlı ters vekil'i (reverse proxy) Varnish ya da Squid
gibi sistemlerle de uygulamanızı kolaylıkla ölçeklendirebilirsiniz.

.. note::

    
    Eğer tüm sayfaları ön bellekleyemezseniz ? Symfony2'nin Edge Side Includes (ESI)
    sistemini doğal olarak desteklediğinden bu soruna zaten bir çözümü vardır.
    Daha fazla bilgi almak için kitabın ":doc:`/book/http_cache`" kısmını okuyun.

Son Sözler
-----------
Hepsinin b kadar olduğundan ve 10 dakika geçirdiğimden emin değilim.
İlk kısımda kısaca bundle 'lara giriş yaptık ve temel framework bundle'ın
neredeyse tüm özelliklerini öğrendik. Bundellar sayesinde herşey genişle
tilebilir ve değiştirilebilir.
Bu öğreticinin :doc:`bundan sonraki başlığı için buradan<the_architecture>`.

