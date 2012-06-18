NotNull
=======

Bir değerin kesin suretle ``null`` 'e eşit olmadığını doğrular. Bir değerin
basitçe boş olmadığından emin olmak için (boş bir dize değil), :doc:`/reference/constraints/NotBlank` 
kısıtına bakınız.

+----------------+-----------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`        |
+----------------+-----------------------------------------------------------------------+
| Seçnekler      | - `message`_                                                          |
+----------------+-----------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\NotNull`          |
+----------------+-----------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\NotNullValidator` |
+----------------+-----------------------------------------------------------------------+

Temel Kullanım
--------------

Bir ``Author`` sınıfının ``firstName`` sınıf değişkeninin kesin suretle boş olmadığından emin olmak istiyorsanız,
şöyle yapabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - NotNull: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotNull()
             */
            protected $firstName;
        }

Seçenekler
----------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value should not be null``

Eğer değer ``null`` ise gösterilecek mesaj bu olacaktır.
