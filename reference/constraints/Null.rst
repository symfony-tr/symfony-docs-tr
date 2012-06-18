Null
====

Bir değerin kesin olarak ``null`` 'e eşit olduğunu doğrular. Bir sınıf değişkeninin
basitçe boş olması geçerliliği için (boş dize ya da ``null``), :doc:`/reference/constraints/Blank`
kısıtına bakınız. Bir sınıf değişkeninin null olmadığından emin olmak için, :doc:`/reference/constraints/NotNull`
'e bakınız.

+----------------+-----------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`        |
+----------------+-----------------------------------------------------------------------+
| Seçenekler     | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\Null`             |
+----------------+-----------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\NullValidator`    |
+----------------+-----------------------------------------------------------------------+

Temel Kullanım
--------------

Eğer, bir nedenden ötürü, `Author`` sınıfının ``firstName`` sınıf değişkeninin
kesin olarak ``null`` 'e eşit olduğundan emin olmak istiyorsanız, aşağıdakini
yapabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                firstName:
                    - Null: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        namespace Acme\BlogBundle\Entity;
        
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Null()
             */
            protected $firstName;
        }

Seçenekler
----------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value should be null``

Eğer değer ``null`` değilse gösterilecek mesaj bu olacaktır.
