Blank
=====

Bir değerin boş olduğunu doğrular, boş bir dizeye eşittir ya da 
``null`` 'e eşittir şeklinde tanımlanmıştır. Bir değerin kesin suretle ``null`` 'e eşit olması geçerliliği için,
:doc:`/reference/constraints/Null` kısıtına bakınız. Bir değerin boş *değil* geçerliliği için,
:doc:`/reference/constraints/NotBlank` 'e bakınız.

+----------------+-----------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metod<validation-property-target>`        |
+----------------+-----------------------------------------------------------------------+
| Seçenekler     | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\Blank`            |
+----------------+-----------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\BlankValidator`   |
+----------------+-----------------------------------------------------------------------+

Temel Kullanım
--------------

Eğer, bir nedenden ötürü, ``Author`` sınıfının ``firstName`` sınıf değişkeninin boş olduğundan emin olmak 
istiyorsanız, aşağıdakini yapabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - Blank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\Blank()
             */
            protected $firstName;
        }

Seçenekler
----------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value should be blank``

Eğer değer boş değilse gösterilecek mesaj bu olacaktır.
