NotBlank
========

Bir değerin boş olmadığını doğrular, boş bir dizeye eşit değildir şeklinde tanımlanmıştır
ve ayrıca ``null`` 'e de eşit değildir. Bir değerin ``null`` 'e basitçe eşit olmaması geçerliliği için,
:doc:`/reference/constraints/NotNull` kısıtına bakınız.

+----------------+------------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metod<validation-property-target>`         |
+----------------+------------------------------------------------------------------------+
| Seçenekler     | - `message`_                                                           |
+----------------+------------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlank`          |
+----------------+------------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\NotBlankValidator` |
+----------------+------------------------------------------------------------------------+

Temel Kullanım
--------------

Bir ``Author`` sınıfının ``firstName`` sınıf değişkeninin boş olmadığından emin olmak istiyorsanız,
aşağıdakini yapabilirsiniz:

.. configuration-block::

    .. code-block:: yaml

        properties:
            firstName:
                - NotBlank: ~

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\NotBlank()
             */
            protected $firstName;
        }

Seçenekler
-------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``Bu değer boş olamaz``

Eğer değer boşsa bu mesaj dönecektir.
