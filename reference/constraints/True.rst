True
====

Bir değerin ``true`` olduğunu doğrular. Özellikle, değerin kesin olarak ``true``,
kesin olarak ``1``, ya da kesin olarak dize "``1``" olup olmadığını kontrol eder.

Ayrıca :doc:`False <False>` 'e bakınız.

+----------------+---------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`      |
+----------------+---------------------------------------------------------------------+
| Seçenekler     | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\True`           |
+----------------+---------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\TrueValidator`  |
+----------------+---------------------------------------------------------------------+

Temel Kullanım
--------------

Bu kısıt sınıf değişkenlerine (örneğin bir kayıt modelinde ``termsAccepted`` sınıf değişkeni) 
ya da bir "getter" metoduna uygulanabilir. Bir metodun true değeri döndürdüğünü ileri sürdüğünüz 
sonraki durumlarda çok daha güçlüdür. Örnek olarak, farz edin ki aşağıdaki metodunuz var:

.. code-block:: php

    // src/Acme/BlogBundle/Entity/Author.php
    namespace Acme\BlogBundle\Entity;

    class Author
    {
        protected $token;

        public function isTokenValid()
        {
            return $this->token == $this->generateToken();
        }
    }

Öyleyse bu metodu ``True`` ile kısıtlayabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            getters:
                tokenValid:
                    - "True": { message: "The token is invalid" }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            protected $token;

            /**
             * @Assert\True(message = "The token is invalid")
             */
            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <!-- src/Acme/Blogbundle/Resources/config/validation.xml -->

        <constraint-mapping xmlns="http://symfony.com/schema/dic/constraint-mapping"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/dic/constraint-mapping http://symfony.com/schema/dic/constraint-mapping/constraint-mapping-1.0.xsd">

            <class name="Acme\BlogBundle\Entity\Author">
                <getter property="tokenValid">
                    <constraint name="True">
                        <option name="message">The token is invalid...</option>
                    </constraint>
                </getter>
            </class>
        </constraint-mapping>

    .. code-block:: php

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Mapping\ClassMetadata;
        use Symfony\Component\Validator\Constraints\True;
        
        class Author
        {
            protected $token;
            
            public static function loadValidatorMetadata(ClassMetadata $metadata)
            {
                $metadata->addGetterConstraint('tokenValid', new True(array(
                    'message' => 'The token is invalid',
                )));
            }

            public function isTokenValid()
            {
                return $this->token == $this->generateToken();
            }
        }

Eğer ``isTokenValid()`` false dönerse, onaylama hatalı olacaktır.

Seçenekler
----------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value should be true``

Eğer vurgulanan veri true değilse bu mesaj gösterilecektir.
