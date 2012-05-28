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
    bu konudaki daha fazla bilgi için :ref:`book-validation-forms` 
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
dolaylı olarak çalışacaksınız. Smyofny'nin form kütüphanesi ``validator``
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
                // doğrulama geçti, $author nesnesiile bir şeyler yap.

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

Symfony2 validator servisi varsayılan olarak açık gelir ancak eğer
kısıtlama kurallarınız içerisinde belirteçleri kullandıysanız bu belirteçleri
açmalısınız:

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

``validator`` nesneleri *kısıtlara* göre doğrulayacak şekilde tasarlanmıştır
(örn : kurallar). Nesnenin doğrulanmasına göre basitçe bir ya da daha
fazla sınıfa ait olan kısıt eşleşir ve ``validator`` hizmetinden geçer.

İşin mutfağında bir kısıt, araya bir ifade sıkıştıran basit bir PHP nesnesidir.
Gerçekte bir kısıt "Kek yanmamalı" olabilir. Symfony2'deki 
kısıtlarda aynırı. Araya sıkıştırdıkları bir şart doğrudur. Bir kısıt
verilen değer için bu değerin kısıt kuralları ile eşleştirilip eşleştirilmediğini
söyleyecektir.

Desteklenen Kısıtlar
~~~~~~~~~~~~~~~~~~~~

Symfony2 geniş bir sayıda en çok gereken kısıt ile birlikte gelir:

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

The options of a constraint can always be passed in as an array. Some constraints,
however, also allow you to pass the value of one, "*default*", option in place
of the array. In the case of the ``Choice`` constraint, the ``choices``
options can be specified in this way.

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

This is purely meant to make the configuration of the most common option of
a constraint shorter and quicker.

If you're ever unsure of how to specify an option, either check the API documentation
for the constraint or play it safe by always passing in an array of options
(the first method shown above).

.. index::
   single: Veri Doğrulama; Constraint targets

.. _validator-constraint-targets:

Constraint Targets
------------------

Constraints can be applied to a class property (e.g. ``name``) or a public
getter method (e.g. ``getFullName``). The first is the most common and easy
to use, but the second allows you to specify more complex validation rules.

.. index::
   single: Veri Doğrulama; Property constraints

.. _validation-property-target:

Properties
~~~~~~~~~~

Validating class properties is the most basic validation technique. Symfony2
allows you to validate private, protected or public properties. The next
listing shows you how to configure the ``$firstName`` property of an ``Author``
class to have at least 3 characters.

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
   single: Veri Doğrulama; Getter constraints

Getters
~~~~~~~

Constraints can also be applied to the return value of a method. Symfony2
allows you to add a constraint to any public method whose name starts with
"get" or "is". In this guide, both of these types of methods are referred
to as "getters".

The benefit of this technique is that it allows you to validate your object
dynamically. For example, suppose you want to make sure that a password field
doesn't match the first name of the user (for security reasons). You can
do this by creating an ``isPasswordLegal`` method, and then asserting that
this method must return ``true``:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                passwordLegal:
                    - "True": { message: "The password cannot match your first name" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\True(message = "The password cannot match your first name")
             */
            public function isPasswordLegal()
            {
                // return true or false
            }
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Author">
            <getter property="passwordLegal">
                <constraint name="True">
                    <option name="message">The password cannot match your first name</option>
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
                    'message' => 'The password cannot match your first name',
                )));
            }
        }

Now, create the ``isPasswordLegal()`` method, and include the logic you need::

    public function isPasswordLegal()
    {
        return ($this->firstName != $this->password);
    }

.. note::

    The keen-eyed among you will have noticed that the prefix of the getter
    ("get" or "is") is omitted in the mapping. This allows you to move the
    constraint to a property with the same name later (or vice versa) without
    changing your validation logic.

.. _validation-class-target:

Classes
~~~~~~~

Some constraints apply to the entire class being validated. For example,
the :doc:`Callback</reference/constraints/Callback>` constraint is a generic
constraint that's applied to the class itself. When that class is validated,
methods specified by that constraint are simply executed so that each can
provide more custom validation.

.. _book-validation-validation-groups:

Validation Groups
-----------------

So far, you've been able to add constraints to a class and ask whether or
not that class passes all of the defined constraints. In some cases, however,
you'll need to validate an object against only *some* of the constraints
on that class. To do this, you can organize each constraint into one or more
"validation groups", and then apply validation against just one group of
constraints.

For example, suppose you have a ``User`` class, which is used both when a
user registers and when a user updates his/her contact information later:

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

With this configuration, there are two validation groups:

* ``Default`` - contains the constraints not assigned to any other group;

* ``registration`` - contains the constraints on the ``email`` and ``password``
  fields only.

To tell the validator to use a specific group, pass one or more group names
as the second argument to the ``validate()`` method::

    $errors = $validator->validate($author, array('registration'));

Of course, you'll usually work with validation indirectly through the form
library. For information on how to use validation groups inside forms, see
:ref:`book-forms-validation-groups`.

.. index::
   single: Veri Doğrulama; Validating raw values

.. _book-validation-raw-values:

Validating Values and Arrays
----------------------------

So far, you've seen how you can validate entire objects. But sometimes, you
just want to validate a simple value - like to verify that a string is a valid
email address. This is actually pretty easy to do. From inside a controller,
it looks like this::

    // add this to the top of your class
    use Symfony\Component\Validator\Constraints\Email;
    
    public function addEmailAction($email)
    {
        $emailConstraint = new Email();
        // all constraint "options" can be set this way
        $emailConstraint->message = 'Invalid email address';

        // use the validator to validate the value
        $errorList = $this->get('validator')->validateValue($email, $emailConstraint);

        if (count($errorList) == 0) {
            // this IS a valid email address, do something
        } else {
            // this is *not* a valid email address
            $errorMessage = $errorList[0]->getMessage()
            
            // do something with the error
        }
        
        // ...
    }

By calling ``validateValue`` on the validator, you can pass in a raw value and
the constraint object that you want to validate that value against. A full
list of the available constraints - as well as the full class name for each
constraint - is available in the :doc:`constraints reference</reference/constraints>`
section .

The ``validateValue`` method returns a :class:`Symfony\\Component\\Validator\\ConstraintViolationList`
object, which acts just like an array of errors. Each error in the collection
is a :class:`Symfony\\Component\\Validator\\ConstraintViolation` object,
which holds the error message on its `getMessage` method.

Final Thoughts
--------------

The Symfony2 ``validator`` is a powerful tool that can be leveraged to
guarantee that the data of any object is "valid". The power behind validation
lies in "constraints", which are rules that you can apply to properties or
getter methods of your object. And while you'll most commonly use the validation
framework indirectly when using forms, remember that it can be used anywhere
to validate any object.

Learn more from the Cookbook
----------------------------

* :doc:`/cookbook/validation/custom_constraint`

.. _Validator: https://github.com/symfony/Validator
.. _JSR303 Bean Veri Doğrulama Şartnamesi: http://jcp.org/en/jsr/detail?id=303
