.. index::
   single: Veri Doğrulama

Veri Doğrulama
==============
Veri doğrulama web uygulamalarındaki en yaygın işlerden bir tanesidir.
Formdan gelen verinin doğrulanması gerekir. Veri ayrıca veritabanına 
yazılmadan önce ya da bir web servisine gönderilmeden önce de doğrulanmalıdır.

Symfony2 bu işi basit ve şeffaf bir şekilde yapan ``Validator``_ bileşeni 
ile birlikte gelir. Bu bileşen `JSR303 Bean Veri Doğrulama Şartnamesi`_ 
baz alınarak geliştirilmiştir. Peki bir Java şarnamesinin PHP'de ne işi var?
Doğru duydunuz fakat göründüğü kadar kötü değil. 
Şimdi bunun PHP'de nasıl kullanıldığına bakalım.

.. index:
   single: Veri Doğrulama; Temeller

Veri Doğrulamanın Temelleri
---------------------------

Veri doğrulamayı en iyi şekilde anlamak için onu uygulamada görmelisiniz.
Başlamak için, varsayalım uygulamanızın herhangi bir yerinde kullanmanız
gereken düz-basit bir PHP nesneniz var:

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        public $name;
    }
Şu ana kadar bu sıradan sınıf uygulamanız içerisinde bazı amaçlara hizmet
ediyor. Veri doğrulamanın amacı verinin ya da sınıfın doğru olup olmadığını
bildirmektir. Bu çalışmada konfigüre edeceğiniz kurallar 
(:ref:`kısıtlar<validation-constraints>` olarak adlandırılır) nesnenin
gerçerli olabilmesi için uyulması gereken kurallardır.
Bu kurallar bir dizi farklı formatta tanımlanabilir 
(YAML, XML, belirteçler(annotation), ya da  PHP).


Örneğin ``$name`` sınıf değişkeninin boş olmamasını garanti altına almak
istiyorsanız şunu ekleyin:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                name:
                    - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            public $name;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="name">
                    <constraint name="NotBlank" />
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $name;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('name', new NotBlank());
            }
        }

.. tip::

    Protected ve private özellikteki sınıf değişkenleri ayrıca "getter" 
    metodları ile de doğrulanabilir (bkz `validator-constraint-targets`). 

.. index::
   single: Veri Doğrulama; Doğrulayıcı (Validator) kullanmak

``validator`` Servisini Kullanmak
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Daha sonra, ``Author`` nesnesinin gerçekten doğrulanması için ``validator``
servisinin ``validate`` metodunu kullanın (:class:`Symfony\\Component\\Validator\\Validator` sınıfı).
``validator`` servisinin işi basittir. Sınıfın kısıtlarını oku (örn: kurallar)
ve  verinin nesnenin bu kısıtlarını karşılayıp karşılamadığını kontrol et.
Eğer doğrulama geçersiz ise bir hatalar array'i döndürülür. Bu basit örneği
bir controller içerisinde görelim:

.. code-block:: php

    use Symfony\Component\HttpFoundation\Response;
    use Acme\BlogBundle\Entity\Author;
    // ...

    public function indexAction()
    {
        $author = new Author();
        // ... $author nesnesi ile bir şeyler yap.

        $validator = $this->get('validator');
        $errors = $validator->validate($author);

        if (count($errors) > 0) {
            return new Response(print_r($errors, true));
        } else {
            return new Response('Yazar Geçerli! Evvet!');
        }
    }


Eğer ``$name`` sınıf değişkeni boş ise şu aşağıdaki hata mesajını
göreceksiniz::

.. code-block:: text

    Acme\BlogBundle\Author.name:
        This value should not be blank

Eğer ``name`` sınıf değişkenine bir değer verilirse başarılı mesajı
gözükecektir.


.. tip::

    Çoğu zaman, direk olarak ``validator`` servisi ile uğraşmak
    istemeyecek ya da bu hataları görmek istemeyeceksiniz. Çoğu
    zaman form verisini işlerken dolaylı olarak doğrulama kullanacaksınız.
    Bu konudaki daha fazla bilgi için :ref:`book-validation-forms` 
    belgesine bakınız.

Ayrıca hataların bir kolleksiyonunu da şablona aktarabilirsiniz.
 
.. code-block:: php

    if (count($errors) > 0) {
        return $this->render('AcmeBlogBundle:Author:validate.html.twig', array(
            'errors' => $errors,
        ));
    } else {
        // ...
    }

Şablon içerisinde bu hataları ihtiyaç halinde açıkça şu şekilde
gösterebilirsiniz.

.. configuration-block::

    .. code-block:: html+jinja

        {# src/Acme/BlogBundle/Resources/views/Author/validate.html.twig #}

        <h3>Yazar için şu hatalara rastlandı</h3>
        <ul>
        {% for error in errors %}
            <li>{{ error.message }}</li>
        {% endfor %}
        </ul>

    .. code-block:: html+php

        <!-- src/Acme/BlogBundle/Resources/views/Author/validate.html.php -->

        <h3>Yazar için şu hatalara rastlandı</h3>
        <ul>
        <?php foreach ($errors as $error): ?>
            <li><?php echo $error->getMessage() ?></li>
        <?php endforeach; ?>
        </ul>

.. note::

    Her doğrulama hatası ("constraint violation" olarak adlandırılır)
    :class:`Symfony\\Component\\Validator\\ConstraintViolation` nesnesi
    tarafından temsil edilir. 

.. index::
   single: Veri Doğrulama; Formlar ile Veri Doğrulama

.. _book-validation-forms:

Veri Doğrulama ve Formlar
~~~~~~~~~~~~~~~~~~~~~~~~~
``validator`` Servisi herhangi bir nesneyi doğrulamak için her zaman
kullanılabilir. Gerçekte , genellikle ``validator`` ile formlarla çalışırken
dolaylı olarak çalışacaksınız. Symfony'nin form kütüphanesi, ``validator``
servisini igili nesne için veriler submit edildikten ve
nesne değişkenine form verileri bindirildikten sonra içsel olarak çalıştırır.
Nesnenin kısıt hataları ``FieldError`` nesnesine çevrilerek form içerisinde
kolaylıkla gösterilir.  Tipik form veri göndermesi akışı controller üzerinde
şu şekilde gözükmektedir:: 

    use Acme\BlogBundle\Entity\Author;
    use Acme\BlogBundle\Form\AuthorType;
    use Symfony\Component\HttpFoundation\Request;
    // ...

    public function updateAction(Request $request)
    {
        $author = new Acme\BlogBundle\Entity\Author();
        $form = $this->createForm(new AuthorType(), $author);

        if ($request->getMethod() == 'POST') {
            $form->bindRequest($request);

            if ($form->isValid()) {
                // doğrulama geçti, $author nesnesi ile bir şeyler yap.

                return $this->redirect($this->generateUrl('...'));
            }
        }

        return $this->render('BlogBundle:Author:form.html.twig', array(
            'form' => $form->createView(),
        ));
    }

.. note::

    Bu örnek burada gösterilmeyen ``AutorType`` form sınıfını kullanır.

Daha fazla bilgi için :doc:`Formlar</book/forms>` bölümüne bakın.

.. index::
   pair: Veri Doğrulama; Configuration

.. _book-validation-configuration:

Konfigürasyon
-------------

Symfony2 validator servisi varsayılan olarak çalışır haldedir. Ancak eğer
kısıtlama kurallarınız içerisinde belirteçleri(annotations) kullandıysanız 
bu belirteçleri çalışır hale getirmelisiniz:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        framework:
            validation: { enable_annotations: true }

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <framework:config>
            <framework:validation enable_annotations="true" />
        </framework:config>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('framework', array('validation' => array(
            'enable_annotations' => true,
        )));

.. index::
   single: Veri Doğrulama; Kısıtlar

.. _validation-constraints:

Kısıtlar
--------

``validator`` Nesneleri *kısıtlara* göre doğrulayacak şekilde tasarlanmıştır
(örn : kurallar). Nesnenin doğrulanmasına göre basitçe bir ya da daha
fazla sınıfa ait olan kısıt eşleşir ve ``validator`` hizmetinden geçer.

İşin mutfağında bir kısıt, araya bir ifade sıkıştıran basit bir PHP nesnesidir.
Gerçekte bir kısıt "Kek yanmamalı" olabilir. Symfony2'deki 
kısıtlarda aynıdır. Araya sıkıştırdıkları bir şart doğrudur. Bir kısıt
verilen değer için bu değerin kısıt kuralları ile eşleştirilip eşleştirilmediğini
söyleyecektir.

Desteklenen Kısıtlar
~~~~~~~~~~~~~~~~~~~~

Symfony2 en sık kullanılan kısıtları barındıran geniş bir liste ile 
birlikte gelir:

.. include:: /reference/constraints/map.rst.inc

Ayrıca kendi kısıtlarınızı da yaratabilisiniz. Bu konu tarif kitabının
":doc:`/cookbook/validation/custom_constraint`" başlığı altında işlenmiştir.

.. index::
   single: Veri Doğrulama; Kısıtların Konfigürasyonu

.. _book-validation-constraint-configuration:

Kısıt Konfigürasyonu
~~~~~~~~~~~~~~~~~~~~~~~~

:doc:`NotBlank</reference/constraints/NotBlank>` gibi bazı kısıtlar 
basit iken :doc:`Choice</reference/constraints/Choice>` gibi kısıtların
pek çok ayar seçeneği bulunmaktadır. 

Varsayalım ki ``Author`` sınıfı ``gender`` adında bir değişkene sahip ve
bu değişken ``male`` ya da ``female`` olarak ayarlanmalı::

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: { choices: [male, female], message: Choose a valid gender. }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice(
             *     choices = { "male", "female" },
             *     message = "Choose a valid gender."
             * )
             */
            public $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <option name="choices">
                            <value>male</value>
                            <value>female</value>
                        </option>
                        <option name="message">Choose a valid gender.</option>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;

        class Author
        {
            public $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array(
                    'choices' => array('male', 'female'),
                    'message' => 'Choose a valid gender.',
                )));
            }
        }

.. _validation-default-option:

Kısıtların seçenekleri daima bir array içerisinde aktarılır. Ancak bazı 
aktaracağınız kısıtlar array içerisinde bir *default* seçeneği bulundururlar.
``Choice`` kısıtı örneğinde ``choices`` seçeneği bu yolla belirlenir.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                gender:
                    - Choice: [male, female]

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Choice({"male", "female"})
             */
            protected $gender;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <property name="gender">
                    <constraint name="Choice">
                        <value>male</value>
                        <value>female</value>
                    </constraint>
                </property>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Choice;

        class Author
        {
            protected $gender;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('gender', new Choice(array('male', 'female')));
            }
        }

Bu konfigürasyonun anlamı açık bir şekilde pek çok seçenek basitçe ve kısaca 
konfigüre edilebilirdir.
Eğer bir seçeneği nasıl belirleyeceğinize emin değilseniz ya bu kısıt için API dokümanına
bakın ya da seçenekleri bir dize içerisinde vererek kurcalayın (yukarıda gösterilen ilk metod).

.. index::
   single: Veri Doğrulama; Kısıt Hedefleri	

.. _validator-constraint-targets:

Kısıt Hedefleri
---------------

Kısıtlar bir nesne değişkenine (örn : ``name``) ya da getter metodlarına
(örn: ``getFullName``) uygulanabilir. Birincisi en sık ve kolay kullanılan 
olanıdır  ancak ikincisi size daha karmaşık doğrulama kurallarını 
uygulamanızı sağlar.

.. index::
   single: Veri Doğrulama; Sınıf Değişkeni Kısıtları

.. _validation-property-target:

Sınıf Değişkenleri (Properties)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Sınıf değişkenlerini doğrulamak en basit veri doğrulama tekniğidir.
Symfony2, private, protected ya da public sınıf değişkenlerini
doğrulayabilir. Aşağıdaki liste ``Author`` sınıfındaki ``$firstName``
değişkeninin nasıl en az 3 karakter olacağını göstermektedir.

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - NotBlank: ~
                    - MinLength: 3

    .. code-block:: php-annotations

        // Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             * @Assert\MinLength(3)
             */
            private $firstName;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <property name="firstName">
                <constraint name="NotBlank" />
                <constraint name="MinLength">3</constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class Author
        {
            private $firstName;

            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('firstName', new NotBlank());
                $metadata->addPropertyConstraint('firstName', new MinLength(3));
            }
        }

.. index::
   single: Veri Doğrulama; Getter Kısıtları

Getter'lar
~~~~~~~~~~

Kısıtlar aynı zamanda metodun dönüş değerine de uygulanabilir. Symfony2
"get" ya da "is" ile başlayan herhangi bir public metoda kısıt eklemenize
izin verir. Bu klavuzda bu iki tip metodun genel adı "getters" olarak 
ifade edilir.

Bu tekniğin avantajı, nesnenizi dinamik olarak veri doğrulama yapma imkanı
sağlamasıdır. Örneğin, varsayalım, bir parola alanınındaki veriyi kullanıcının
adı ile aynı olmamasını istiyorsunuz(güvenlik nedenleri ile). Bunu ``isPasswordLegal``
metodu yaratarak bunun ``true`` dönmesini durumunda bir kısıt sağlayabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - "True": { message: "Parola isminiz ile aynı olamaz" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "Parola isminiz ile aynı olamaz")
             */
            public function isPasswordLegal()
            {
                // true ya da false döner
            }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">Parola isminiz ile aynı olamaz</option>
                </constraint>
            </getter>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;

        class Author
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('passwordLegal', new True(array(
                    'message' => 'Parola isminiz ile aynı olamaz',
                )));
            }
        }

Şimdi, ``isPasswordLegal()`` metodu yaratarak gerekli algoritmayı içerisine
ekleyelim::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    Keskin bir gözünüz varsa hemen getter ön eklerinin ("get" ya da "is")
    eşleştirmenin dışında olduğunu farkedecektir. Bu size sınıf değişkeni
    kısıtının aynı isimle daha sonra doğrulama algoritmasını değiştirmeden
    taşımanıza imkan sağlar.

.. _validation-class-target:

Sınıflar
~~~~~~~

Bazı kısıtlamalar veri doğrulama yapılacak sınıfın tamamına uygulanır.
Örneğin :doc:`Callback</reference/constraints/Callback>` kısıtı,
sınıfın kendisine uygulanan jenerik bir kısıttır. Sınıf doğrulandığında
bu kısıtlar tarafından belirlenmiş metodlar çalışacak ve her birisi
size daha fazla özelleştirilmiş bir veri doğrulama sağlayacaktır.

.. _book-validation-validation-groups:

Kısıt Gurupları
---------------

Buraya kadar sınıfa kısıtlar atayabilir ve bu sınıfın tüm belirlenen kısıtlardan
geçip geçmediğini öğrenebilirsiniz. Ancak bazı durumlarda bir nesnenin sadece
*bazı* kısıtlar ile doğrulanmasını isteyebilirsiniz. Bunu yapmak için her kısıtını bir
ya da daha fazla "kısıt gurubu" (validation group) içerisine alıp nesneye
sadece bu guruptaki kısıtları uygulayabilirsiniz. 

Örneğin, varsayalım ki kullanıcıların kayıt edilmesine ve kullanıcıların
kayıtlarının sonradan güncellenebilmesine yarayan bir ``User`` sınıfınız var:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\User:
            properties:
                email:
                    - Email: { groups: [registration] }
                password:
                    - NotBlank: { groups: [registration] }
                    - MinLength: { limit: 7, groups: [registration] }
                city:
                    - MinLength: 2

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Security\Core\User\UserInterface;
        use Symfony\Component\Validator\Constraints as Assert;

        class User implements UserInterface
        {
            /**
            * @Assert\Email(groups={"registration"})
            */
            private $email;

            /**
            * @Assert\NotBlank(groups={"registration"})
            * @Assert\MinLength(limit=7, groups={"registration"})
            */
            private $password;

            /**
            * @Assert\MinLength(2)
            */
            private $city;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\User">
            <property name="email">
                <constraint name="Email">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="password">
                <constraint name="NotBlank">
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
                <constraint name="MinLength">
                    <option name="limit">7</option>
                    <option name="groups">
                        <value>registration</value>
                    </option>
                </constraint>
            </property>
            <property name="city">
                <constraint name="MinLength">7</constraint>
            </property>
        </class>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/User.php
        namespace Acme\BlogBundle\Entity;

        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\Email;
        use Symfony\Component\Validator\Constraints\NotBlank;
        use Symfony\Component\Validator\Constraints\MinLength;

        class User
        {
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addPropertyConstraint('email', new Email(array(
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('password', new NotBlank(array(
                    'groups' => array('registration')
                )));
                $metadata->addPropertyConstraint('password', new MinLength(array(
                    'limit'  => 7,
                    'groups' => array('registration')
                )));

                $metadata->addPropertyConstraint('city', new MinLength(3));
            }
        }

Bu konfigürasyonda iki adet veri dogrulama gurubu vardır:

* ``Default`` - herhangi bir diğer guruba dahil edilmeyen kısıtlar;

* ``registration`` -  sadece ``email`` ve ``password`` alanlarını kapsayan
  kısıtlar. 

Validator'a (veri doğrulayıcı) belirli bir gurup kullanımasını söylemek için
bir ya da daha fazla gurup adını ``validate()`` metodunun ikinci argümanında
belirmek gerekilidir::

    $errors = $validator->validate($author, array('registration'));

Elbette, genellikle veri doğrulama ile form kütüphanelerinde dolaylı
olarak çalışacaksınız. Veri doğrulama guruplarının formlar içerisinde
kullanımı hakkındaki bilgiyi :ref:`book-forms-validation-groups` kısmından
alabilirsiniz.

.. index::
   single: Veri Doğrulama; İşlenmemiş Verileri Doğrulama

.. _book-validation-raw-values:

Değerleri ve Dizeleri Doğrulamak
--------------------------------

Buraya kadar tüm bir nesnenin nasıl doğrulanacağını gördünüz. Fakat
bazen sadece bir metin ya da geçerli bir e-posta adresi gibi basit
bir değeri doğrulatmak isteyebilirsiniz. Bunu yapmak gerçekten oldukça
basittir. Controller içerisindeki form şu şekilde düzenlenmelidir::

    // bunu sınıfınızın üzerine ekleyin
    use Symfony\Component\Validator\Constraints\Email;
    
    public function addEmailAction($email)
    {
        $emailConstraint = new Email();
        // tüm kısıt "seçenekleri" bu yolla ayarlanabilir.
        $emailConstraint->message = 'Geçersiz e-posta adresi';

        //  validator'u kullanarak değeri doğrula
        $errorList = $this->get('validator')->validateValue($email, $emailConstraint);

        if (count($errorList) == 0) {
            // eğer bu "geçerli" bir e-posta adresi ise
        } else {
            // bu geçerli bir e-posta adresi "değil" ise
            $errorMessage = $errorList[0]->getMessage()
            
            // bu hata ile bir şeyler yap
        }
        
        // ...
    }

Validator'deki ``validateValue`` çağırılıp bir kısıt nesnesine bu 
işlenmemiş değer ile birlikte verilerek değeri doğrulatabilirsiniz.
Tüm mevcut kısıtların tam listesi için -ve her kısıtın tam nesne adları
için :doc:`kısıt referansları</reference/constraints>` belgesine bakın.

``validateValue`` metodu  tüm hataların bir dize değişkeni içerisinde 
:class:`Symfony\\Component\\Validator\\ConstraintViolationList` sınıfı olarak 
döndürür. Kolleksiyondaki her hata, :class:`Symfony\\Component\\Validator\\ConstraintViolation` 
nesnesinde tutulur ve bu hata mesajlarına `getMessage` metodu ile erişirsiniz.

Son Düşünceler
--------------

Symfony2 ``validator``  (veri doğrulayıcı) herhangi bir nesnenin verisinin
"geçerli" olduğunu garantilemek için kullanılan güçlü bir araçtır. Veri
doğrulamanın arkasında yatan güç, sınıf değişkenlerine ya da nesnenizin
getter metodlarına uygulayabileceğiniz "kısıtlar" dır. Veri doğrulamada 
en çok kullanılan yöntem formlardaki dolaylı kullanımdır. Şunu unutmayın ki
veri doğrulama herhangibir nesne içerisinde kullanılabilir.

Tarif Kitabından Daha Fazlasını Öğrenin
---------------------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _JSR303 Bean Veri Doğrulama Şartnamesi: http://jcp.org/en/jsr/detail?id=303
