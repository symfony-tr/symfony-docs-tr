.. index::
   single: Testler

Testler
=======

Yeni bir satır kod yazdığınızda potanstiyel olarak yeni hatalar (bug) eklemiş
olursunuz. Daha iyi bir güvenilir uygulamalar geliştirmek için 
kodunuzu fonksiyonel ve unit testleri ile test etmelisiniz.

PHPUnit Test Framework'u
-----------------------------

Symfony2 bağımsız bir kütüphane entegrasyonu ile - PHPUnit olarak adlandırılan -
size zengin bir test framework'u sunar. Bu kısım kendsinin mükemmel bir `belge`_ 'si
olduğundan PHPUnit'in kendisini incelemeyecektir.

.. note::

    Symfony2 PHPUnit 3.5.11 ya da daha üst sürümü ile çalışır.

Her test - unit testi  ya da fonksiyonel bir test - bundle'nızın `Tests/'
klasöründe bulunması gereken bir PHP sınıfıdır. Eğer bu kurala uyarsanız,
uygulamanızın tüm testlerini aşağıdaki komut yardımı ile yapabilirsiniz:

.. code-block:: bash


    # konfigürasyon dizininizi komut satırında belirle
    $ phpunit -c app/

``-c`` seçeneği PHPUnit'e konfigürasyon dosyası için ``app/`` klasörüne bakmasını 
söyler. Eğer PHPUnit seçenekleri hakkında meraklı iseniz ``app/phpunit.xml.dist`` 
dosyasını inceleyin.

.. tip::

	Kod kapsamı ``--coverage-html`` seçeneği ile yaratılabilir.

.. index::
   single: Tests; Unit Testleri

Unit Testleri
-------------

Bir unit testi genellikle belirli bir PHP sınıfına karşı kullanılır. Eğer
tüm uygulamanıza test uygulamak istiyorsanız `Fonksiyonel Testler`_ kısmına bakın.

Symfony2 unit test yazmak standart PHPUnit unit testi yazmaktak farklı değildir.
Varsayalım bundle'ınız içerisindeki ``Utility/`` dizininde ``Calculator``
adında *oldukça* basit bir sınıfınız var::

    // src/Acme/DemoBundle/Utility/Calculator.php
    namespace Acme\DemoBundle\Utility;
    
    class Calculator
    {
        public function add($a, $b)
        {
            return $a + $b;
        }
    }

Bunu test etmek için bundle'ınız içerisinde ``Tests/Utility`` klasörü altında 
``CalculatorTest`` adında bir dosya yaratın::

    // src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php
    namespace Acme\DemoBundle\Tests\Utility;

    use Acme\DemoBundle\Utility\Calculator;

    class CalculatorTest extends \PHPUnit_Framework_TestCase
    {
        public function testAdd()
        {
            $calc = new Calculator();
            $result = $calc->add(30, 12);

            // iddia ediyoruz calculator rakkamları doğru bir şekilde ekliyor!
            $this->assertEquals(42, $result);
        }
    }

.. note::

    Kural olarak ``Tests/`` alt dizini bundle'ınızın klasörünü kopyalamalıdır.
    Bu yüzden eğer bundle'ınız içerisindeki ``Utility/`` klasörü altında 
    bulunan bir sınıfı test etmek için bu testi ``Tests/Utility/`` klasörüne
    koymalısınız.

Aynı gerçek uygulamanızdaki gibi autoloading ``bootstrap.php.cache`` dosyası
tarafından otomatik olarak aktifleştirilir (eğer ``phpunit.xml.dist``
içerisinde konfigüre edildi ise).

Veriken dosya ya da klasör için testleri çalıştırmak oldukça
kolaydır:

.. code-block:: bash

    # Utility klasöründeki tüm testleri çalıştır.
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/

    # Calculator sınıfı için testleri çalıştır.
    $ phpunit -c app src/Acme/DemoBundle/Tests/Utility/CalculatorTest.php

    # Bundle'ın tamamında testleri çalıştır.
    $ phpunit -c app src/Acme/DemoBundle/

.. index::
   single: Tests; Fonksiyonel Testler

Fonksiyonel Testler
-------------------

Fonksiyonel testler uygulamanızın farklı katmanlarının entegrasyonunu 
test eder(routing'den views katmanına kadar). Bunların PHPUnit'in yaptığı
unit testlerden hiç bir farkı yoktur ancak oldukça özel bir akışları vardır.

* Bir istek yap (request);
* Cevabı (response) test et;
* Bir linke tıkla ya da bir form verisi gönder;
* Cevabı (response) test et;
* Başa al ve tekrarla;

İlk Fonksiyonel Tesiniz
~~~~~~~~~~~~~~~~~~~~~~~

Fonksiyonel testler genel olarak bundle'ınızın ``Tests/Controller`` 
dizininde bulunan basit PHP dosyalarıdır. Eğer ``DemoController`` class'ınızı
işleten sayfanızı test etmek istiyorsanız yeni bir ``DemoControllerTest.php``
dosyası yaratıp bunu özel ``WebTestCase`` sınıfından genişleterek başlayın.

Örneğin Symfony2 Standart sürüm aşağıdaki gibi ``DemoController`` 
için basit bir fonksiyonel test ile (`DemoControllerTest`_) birlikte gelir::

    // src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
    namespace Acme\DemoBundle\Tests\Controller;

    use Symfony\Bundle\FrameworkBundle\Test\WebTestCase;

    class DemoControllerTest extends WebTestCase
    {
        public function testIndex()
        {
            $client = static::createClient();

            $crawler = $client->request('GET', '/demo/hello/Fabien');

            $this->assertGreaterThan(0, $crawler->filter('html:contains("Hello Fabien")')->count());
        }
    }

.. tip::

    Fonksiyonel testleri çalıştırmak için uygulamanızın çekirdeğinde ``WebTestCase``
    sınıfı başlatılma esnasında aktif durumda olmalıdır. Pek çok durumda bu otomatik
    olarak olur. Ancak eğer çekirdeğiniz standart olmayan bir klasörde ise
    ``phpunit.xml.dist`` dosyasındaki ``KERNEL_DIR`` çevre değişkenini kernelinizin
    bulunduğu yere göre düzenlemeniz gerekir::

        <phpunit>
            <!-- ... -->
            <php>
                <server name="KERNEL_DIR" value="/path/to/your/app/" />
            </php>
            <!-- ... -->
        </phpunit>

``createClient()`` metodu bir tarayıcıdan sitenizden bilgi alıyormuş 
gibi davranır::

    $crawler = $client->request('GET', '/demo/hello/Fabien');

``request()`` metodu (bkz :ref:`request metodu hakkında daha fazlası için<book-testing-request-method-sidebar>`)
Response esnasında linklere tıklamak ve form verisi gönderme durumunda nesneleri seçmekte
kullanılan bir  :class:`Symfony\\Component\\DomCrawler\\Crawler` nesnesini döndürür.

.. tip::

    Crawler sadece response XML ya da HTML dokümanı ise çalışır.
    İşlenmemiş response içeriği almak için ``$client->getResponse()->getContent()``
    metodunu çağırın.

Crawler ile seçilen bir XPath ya da CSS seçiçisi kullanan bir linke tıklayın,
sonra istemiciyi ona tıklamak için kullanın. Örneğin aşağıdaki kod, içinde
``Greet`` geçen tüm linkleri bulur sonra ikincisini seçer ve ona tıklar::

    $link = $crawler->filter('a:contains("Greet")')->eq(1)->link();

    $crawler = $client->click($link);

Form verisi göndermekte aynıdır. Bir form butonu seçilir, opsiyonel olarak
bazı form verileri değiştirilir ve ilgili form verisi gönderilir (submit)::

    $form = $crawler->selectButton('submit')->form();

    // Bazı değerleri ata
    $form['name'] = 'Lucas';
    $form['form_name[subject]'] = 'Hey there!';

    // formu gönder
    $crawler = $client->submit($form);

.. tip::

    Form ayrıca dosya yüklemelerini işleyen ve bunları dolduran çeşitli tiplerdeki
    form alanlarına sahiptir (Örn: ``select()`` ve ``tick()``). Detaylı bilgi
    için aşağıdaki `Formlar`_ kısmına bakın.

Now that you can easily navigate through an application, use assertions to test
that it actually does what you expect it to. Use the Crawler to make assertions
on the DOM::

    // Assert that the response matches a given CSS selector.
    $this->assertGreaterThan(0, $crawler->filter('h1')->count());

Or, test against the Response content directly if you just want to assert that
the content contains some text, or if the Response is not an XML/HTML
document::

    $this->assertRegExp('/Hello Fabien/', $client->getResponse()->getContent());

.. _book-testing-request-method-sidebar:

.. sidebar:: More about the ``request()`` method:

    The full signature of the ``request()`` method is::

        request(
            $method,
            $uri, 
            array $parameters = array(), 
            array $files = array(), 
            array $server = array(), 
            $content = null, 
            $changeHistory = true
        )

    The ``server`` array is the raw values that you'd expect to normally
    find in the PHP `$_SERVER`_ superglobal. For example, to set the `Content-Type`
    and `Referer` HTTP headers, you'd pass the following::

        $client->request(
            'GET',
            '/demo/hello/Fabien',
            array(),
            array(),
            array(
                'CONTENT_TYPE' => 'application/json',
                'HTTP_REFERER' => '/foo/bar',
            )
        );

.. index::
   single: Tests; Assertions

.. sidebar:: Useful Assertions

    To get you started faster, here is a list of the most common and
    useful test assertions::

        // Assert that there is more than one h2 tag with the class "subtitle"
        $this->assertGreaterThan(0, $crawler->filter('h2.subtitle')->count());

        // Assert that there are exactly 4 h2 tags on the page
        $this->assertCount(4, $crawler->filter('h2')->count());

        // Assert that the "Content-Type" header is "application/json"
        $this->assertTrue($client->getResponse()->headers->contains('Content-Type', 'application/json'));

        // Assert that the response content matches a regexp.
        $this->assertRegExp('/foo/', $client->getResponse()->getContent());

        // Assert that the response status code is 2xx
        $this->assertTrue($client->getResponse()->isSuccessful());
        // Assert that the response status code is 404
        $this->assertTrue($client->getResponse()->isNotFound());
        // Assert a specific 200 status code
        $this->assertEquals(200, $client->getResponse()->getStatusCode());

        // Assert that the response is a redirect to /demo/contact
        $this->assertTrue($client->getResponse()->isRedirect('/demo/contact'));
        // or simply check that the response is a redirect to any URL
        $this->assertTrue($client->getResponse()->isRedirect());

.. index::
   single: Tests; Client

Working with the Test Client
-----------------------------

The Test Client simulates an HTTP client like a browser and makes requests
into your Symfony2 application::

    $crawler = $client->request('GET', '/hello/Fabien');

The ``request()`` method takes the HTTP method and a URL as arguments and
returns a ``Crawler`` instance.

Use the Crawler to find DOM elements in the Response. These elements can then
be used to click on links and submit forms::

    $link = $crawler->selectLink('Go elsewhere...')->link();
    $crawler = $client->click($link);

    $form = $crawler->selectButton('validate')->form();
    $crawler = $client->submit($form, array('name' => 'Fabien'));

The ``click()`` and ``submit()`` methods both return a ``Crawler`` object.
These methods are the best way to browse your application as it takes care
of a lot of things for you, like detecting the HTTP method from a form and
giving you a nice API for uploading files.

.. tip::

    You will learn more about the ``Link`` and ``Form`` objects in the
    :ref:`Crawler<book-testing-crawler>` section below.

The ``request`` method can also be used to simulate form submissions directly
or perform more complex requests::

    // Directly submit a form (but using the Crawler is easier!)
    $client->request('POST', '/submit', array('name' => 'Fabien'));

    // Form submission with a file upload
    use Symfony\Component\HttpFoundation\File\UploadedFile;

    $photo = new UploadedFile(
        '/path/to/photo.jpg',
        'photo.jpg',
        'image/jpeg',
        123
    );
    // or
    $photo = array(
        'tmp_name' => '/path/to/photo.jpg',
        'name' => 'photo.jpg',
        'type' => 'image/jpeg',
        'size' => 123,
        'error' => UPLOAD_ERR_OK
    );
    $client->request(
        'POST',
        '/submit',
        array('name' => 'Fabien'),
        array('photo' => $photo)
    );

    // Perform a DELETE requests, and pass HTTP headers
    $client->request(
        'DELETE',
        '/post/12',
        array(),
        array(),
        array('PHP_AUTH_USER' => 'username', 'PHP_AUTH_PW' => 'pa$$word')
    );

Last but not least, you can force each request to be executed in its own PHP
process to avoid any side-effects when working with several clients in the same
script::

    $client->insulate();

Browsing
~~~~~~~~

The Client supports many operations that can be done in a real browser::

    $client->back();
    $client->forward();
    $client->reload();

    // Clears all cookies and the history
    $client->restart();

Accessing Internal Objects
~~~~~~~~~~~~~~~~~~~~~~~~~~

If you use the client to test your application, you might want to access the
client's internal objects::

    $history   = $client->getHistory();
    $cookieJar = $client->getCookieJar();

You can also get the objects related to the latest request::

    $request  = $client->getRequest();
    $response = $client->getResponse();
    $crawler  = $client->getCrawler();

If your requests are not insulated, you can also access the ``Container`` and
the ``Kernel``::

    $container = $client->getContainer();
    $kernel    = $client->getKernel();

Accessing the Container
~~~~~~~~~~~~~~~~~~~~~~~

It's highly recommended that a functional test only tests the Response. But
under certain very rare circumstances, you might want to access some internal
objects to write assertions. In such cases, you can access the dependency
injection container::

    $container = $client->getContainer();

Be warned that this does not work if you insulate the client or if you use an
HTTP layer. For a list of services available in your application, use the
``container:debug`` console task.

.. tip::

    If the information you need to check is available from the profiler, use
    it instead.

Accessing the Profiler Data
~~~~~~~~~~~~~~~~~~~~~~~~~~~

On each request, the Symfony profiler collects and stores a lot of data about
the internal handling of that request. For example, the profiler could be
used to verify that a given page executes less than a certain number of database
queries when loading.

To get the Profiler for the last request, do the following::

    $profile = $client->getProfile();

For specific details on using the profiler inside a test, see the
:doc:`/cookbook/testing/profiling` cookbook entry.

Redirecting
~~~~~~~~~~~

When a request returns a redirect response, the client does not follow
it automatically. You can examine the response and force a redirection
afterwards  with the ``followRedirect()`` method::

    $crawler = $client->followRedirect();
    
If you want the client to automatically follow all redirects, you can 
force him with the ``followRedirects()`` method::

    $client->followRedirects();

.. index::
   single: Tests; Crawler

.. _book-testing-crawler:

The Crawler
-----------

A Crawler instance is returned each time you make a request with the Client.
It allows you to traverse HTML documents, select nodes, find links and forms.

Traversing
~~~~~~~~~~

Like jQuery, the Crawler has methods to traverse the DOM of an HTML/XML
document. For example, the following finds all ``input[type=submit]`` elements,
selects the last one on the page, and then selects its immediate parent element::

    $newCrawler = $crawler->filter('input[type=submit]')
        ->last()
        ->parents()
        ->first()
    ;

Many other methods are also available:

+------------------------+----------------------------------------------------+
| Method                 | Description                                        |
+========================+====================================================+
| ``filter('h1.title')`` | Nodes that match the CSS selector                  |
+------------------------+----------------------------------------------------+
| ``filterXpath('h1')``  | Nodes that match the XPath expression              |
+------------------------+----------------------------------------------------+
| ``eq(1)``              | Node for the specified index                       |
+------------------------+----------------------------------------------------+
| ``first()``            | First node                                         |
+------------------------+----------------------------------------------------+
| ``last()``             | Last node                                          |
+------------------------+----------------------------------------------------+
| ``siblings()``         | Siblings                                           |
+------------------------+----------------------------------------------------+
| ``nextAll()``          | All following siblings                             |
+------------------------+----------------------------------------------------+
| ``previousAll()``      | All preceding siblings                             |
+------------------------+----------------------------------------------------+
| ``parents()``          | Returns the parent nodes                           |
+------------------------+----------------------------------------------------+
| ``children()``         | Returns children nodes                             |
+------------------------+----------------------------------------------------+
| ``reduce($lambda)``    | Nodes for which the callable does not return false |
+------------------------+----------------------------------------------------+

Since each of these methods returns a new ``Crawler`` instance, you can
narrow down your node selection by chaining the method calls::

    $crawler
        ->filter('h1')
        ->reduce(function ($node, $i)
        {
            if (!$node->getAttribute('class')) {
                return false;
            }
        })
        ->first();

.. tip::

    Use the ``count()`` function to get the number of nodes stored in a Crawler:
    ``count($crawler)``

Extracting Information
~~~~~~~~~~~~~~~~~~~~~~

The Crawler can extract information from the nodes::

    // Returns the attribute value for the first node
    $crawler->attr('class');

    // Returns the node value for the first node
    $crawler->text();

    // Extracts an array of attributes for all nodes (_text returns the node value)
    // returns an array for each element in crawler, each with the value and href
    $info = $crawler->extract(array('_text', 'href'));

    // Executes a lambda for each node and return an array of results
    $data = $crawler->each(function ($node, $i)
    {
        return $node->attr('href');
    });

Links
~~~~~

To select links, you can use the traversing methods above or the convenient
``selectLink()`` shortcut::

    $crawler->selectLink('Click here');

This selects all links that contain the given text, or clickable images for
which the ``alt`` attribute contains the given text. Like the other filtering
methods, this returns another ``Crawler`` object.

Once you've selected a link, you have access to a special ``Link`` object,
which has helpful methods specific to links (such as ``getMethod()`` and
``getUri()``). To click on the link, use the Client's ``click()`` method
and pass it a ``Link`` object::

    $link = $crawler->selectLink('Click here')->link();

    $client->click($link);

Forms
~~~~~

Just like links, you select forms with the ``selectButton()`` method::

    $buttonCrawlerNode = $crawler->selectButton('submit');

.. note::

    Notice that we select form buttons and not forms as a form can have several
    buttons; if you use the traversing API, keep in mind that you must look for a
    button.

The ``selectButton()`` method can select ``button`` tags and submit ``input``
tags. It uses several different parts of the buttons to find them:

* The ``value`` attribute value;

* The ``id`` or ``alt`` attribute value for images;

* The ``id`` or ``name`` attribute value for ``button`` tags.

Once you have a Crawler representing a button, call the ``form()`` method
to get a ``Form`` instance for the form wrapping the button node::

    $form = $buttonCrawlerNode->form();

When calling the ``form()`` method, you can also pass an array of field values
that overrides the default ones::

    $form = $buttonCrawlerNode->form(array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

And if you want to simulate a specific HTTP method for the form, pass it as a
second argument::

    $form = $buttonCrawlerNode->form(array(), 'DELETE');

The Client can submit ``Form`` instances::

    $client->submit($form);

The field values can also be passed as a second argument of the ``submit()``
method::

    $client->submit($form, array(
        'name'              => 'Fabien',
        'my_form[subject]'  => 'Symfony rocks!',
    ));

For more complex situations, use the ``Form`` instance as an array to set the
value of each field individually::

    // Change the value of a field
    $form['name'] = 'Fabien';
    $form['my_form[subject]'] = 'Symfony rocks!';

There is also a nice API to manipulate the values of the fields according to
their type::

    // Select an option or a radio
    $form['country']->select('France');

    // Tick a checkbox
    $form['like_symfony']->tick();

    // Upload a file
    $form['photo']->upload('/path/to/lucas.jpg');

.. tip::

    You can get the values that will be submitted by calling the ``getValues()``
    method on the ``Form`` object. The uploaded files are available in a
    separate array returned by ``getFiles()``. The ``getPhpValues()`` and
    ``getPhpFiles()`` methods also return the submitted values, but in the
    PHP format (it converts the keys with square brackets notation - e.g.
    ``my_form[subject]`` - to PHP arrays).

.. index::
   pair: Tests; Configuration

Testing Configuration
---------------------

The Client used by functional tests creates a Kernel that runs in a special
``test`` environment. Since Symfony loads the ``app/config/config_test.yml``
in the ``test`` environment, you can tweak any of your application's settings
specifically for testing.

For example, by default, the swiftmailer is configured to *not* actually
deliver emails in the ``test`` environment. You can see this under the ``swiftmailer``
configuration option:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config_test.yml
        # ...

        swiftmailer:
            disable_delivery: true

    .. code-block:: xml

        <!-- app/config/config_test.xml -->
        <container>
            <!-- ... -->

            <swiftmailer:config disable-delivery="true" />
        </container>

    .. code-block:: php

        // app/config/config_test.php
        // ...

        $container->loadFromExtension('swiftmailer', array(
            'disable_delivery' => true
        ));

You can also use a different environment entirely, or override the default
debug mode (``true``) by passing each as options to the ``createClient()``
method::

    $client = static::createClient(array(
        'environment' => 'my_test_env',
        'debug'       => false,
    ));

If your application behaves according to some HTTP headers, pass them as the
second argument of ``createClient()``::

    $client = static::createClient(array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

You can also override HTTP headers on a per request basis::

    $client->request('GET', '/', array(), array(), array(
        'HTTP_HOST'       => 'en.example.com',
        'HTTP_USER_AGENT' => 'MySuperBrowser/1.0',
    ));

.. tip::

    The test client is available as a service in the container in the ``test``
    environment (or wherever the :ref:`framework.test<reference-framework-test>`
    option is enabled). This means you can override the service entirely
    if you need to.

.. index::
   pair: PHPUnit; Configuration

PHPUnit Configuration
~~~~~~~~~~~~~~~~~~~~~

Each application has its own PHPUnit configuration, stored in the
``phpunit.xml.dist`` file. You can edit this file to change the defaults or
create a ``phpunit.xml`` file to tweak the configuration for your local machine.

.. tip::

    Store the ``phpunit.xml.dist`` file in your code repository, and ignore the
    ``phpunit.xml`` file.

By default, only the tests stored in "standard" bundles are run by the
``phpunit`` command (standard being tests in the ``src/*/Bundle/Tests`` or
``src/*/Bundle/*Bundle/Tests`` directories) But you can easily add more
directories. For instance, the following configuration adds the tests from
the installed third-party bundles:

.. code-block:: xml

    <!-- hello/phpunit.xml.dist -->
    <testsuites>
        <testsuite name="Project Test Suite">
            <directory>../src/*/*Bundle/Tests</directory>
            <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
        </testsuite>
    </testsuites>

To include other directories in the code coverage, also edit the ``<filter>``
section:

.. code-block:: xml

    <filter>
        <whitelist>
            <directory>../src</directory>
            <exclude>
                <directory>../src/*/*Bundle/Resources</directory>
                <directory>../src/*/*Bundle/Tests</directory>
                <directory>../src/Acme/Bundle/*Bundle/Resources</directory>
                <directory>../src/Acme/Bundle/*Bundle/Tests</directory>
            </exclude>
        </whitelist>
    </filter>

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/testing/http_authentication`
* :doc:`/cookbook/testing/insulating_clients`
* :doc:`/cookbook/testing/profiling`


.. _`DemoControllerTest`: https://github.com/symfony/symfony-standard/blob/master/src/Acme/DemoBundle/Tests/Controller/DemoControllerTest.php
.. _`$_SERVER`: http://php.net/manual/en/reserved.variables.server.php
.. _`belge`: http://www.phpunit.de/manual/3.5/en/
