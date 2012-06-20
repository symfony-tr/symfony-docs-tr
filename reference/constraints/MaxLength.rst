MaxLength
=========

Bir dizenin uzunluğunun verilen limitten büyük olmadığını doğrular.

+----------------+-------------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`          |
+----------------+-------------------------------------------------------------------------+
| Seçenekler     | - `limit`_                                                              |
|                | - `message`_                                                            |
|                | - `charset`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\MaxLength`          |
+----------------+-------------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\MaxLengthValidator` |
+----------------+-------------------------------------------------------------------------+

Temel Kullanım
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Blog:
            properties:
                summary:
                    - MaxLength: 100
    
    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Blog.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MaxLength(100)
             */
            protected $summary;
        }
    
    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Blog">
            <property name="summary">
                <constraint name="MaxLength">
                    <value>100</value>
                </constraint>
            </property>
        </class>

Seçenekler
----------

limit
~~~~~

**tip**: ``integer`` [:ref:`varsayılan seçenek<validation-default-option>`]

Bu gerekli seçenek en büyük değerdir. Verilen dizenin uzunluğu bu sayıdan **fazlaysa**
doğrulama hatalı olacaktır.

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value is too long. It should have {{ limit }} characters or less``

Vurgulanan dizenin uzunluğu `limit`_ seçeneğinden fazlaysa bu mesaj
gösterilecektir.

charset
~~~~~~~

**tip**: ``charset`` **varsayılan**: ``UTF-8``

Eğer "mbstring" PHP uzantısı yüklenmişse, dizenin uzunluğunu hesaplamak için
PHP fonksiyonu `mb_strlen`_ kullanılacaktır. ``charset`` seçeneğinin değeri
bu fonksiyona ikinci bağımsız değişken olarak geçilir.

.. _`mb_strlen`: http://php.net/manual/en/function.mb-strlen.php