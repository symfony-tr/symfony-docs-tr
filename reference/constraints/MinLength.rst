MinLength
=========

Bir dizenin uzunluğunun en az verilen limt kadar olduğunu doğrular.

+----------------+-------------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`          |
+----------------+-------------------------------------------------------------------------+
| Seçenekler     | - `limit`_                                                              |
|                | - `message`_                                                            |
|                | - `charset`_                                                            |
+----------------+-------------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\MinLength`          |
+----------------+-------------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\MinLengthValidator` |
+----------------+-------------------------------------------------------------------------+

Temel Kullanım
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/Acme/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Blog:
            properties:
                firstName:
                    - MinLength: { limit: 3, message: "Your name must have at least {{ limit }} characters." }

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Blog.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Blog
        {
            /**
             * @Assert\MinLength(
             *     limit=3,
             *     message="Your name must have at least {{ limit }} characters."
             * )
             */
            protected $summary;
        }

    .. code-block:: xml

        <!-- src/Acme/BlogBundle/Resources/config/validation.xml -->
        <class name="Acme\BlogBundle\Entity\Blog">
            <property name="summary">
                <constraint name="MinLength">
                    <option name="limit">3</option>
                    <option name="message">Your name must have at least {{ limit }} characters.</option>
                </constraint>
            </property>
        </class>

Seçenekler
----------

limit
~~~~~

**tip**: ``integer`` [:ref:`varsayılan seçenek<validation-default-option>`]

Bu gerekli seçenek en küçük değerdir. Verilen dizenin uzunluğu bu sayıdan **azsa**
doğrulama hatalı olacaktır.

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value is too short. It should have {{ limit }} characters or more``

Vurgulanan dizenin uzunluğu `limit`_ seçeneğinden kısaysa bu mesaj
gösterilecektir.

charset
~~~~~~~

**tip**: ``charset`` **varsayılan**: ``UTF-8``

Eğer "mbstring" PHP uzantısı yüklenmişse, dizenin uzunluğunu hesaplamak için
PHP fonksiyonu `mb_strlen`_ kullanılacaktır. ``charset`` seçeneğinin değeri
bu fonksiyona ikinci bağımsız değişken olarak geçilir.

.. _`mb_strlen`: http://php.net/manual/en/function.mb-strlen.php
