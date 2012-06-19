Type
====

Bir verinin belirli bir tipte olduğunu doğrular. Örnek olarak, eğer bir değer bir dizi olmalıysa,
siz bu kısıtı ``array`` tipi seçeneğiyle doğrulayabilirsiniz.

+----------------+---------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`      |
+----------------+---------------------------------------------------------------------+
| Seçenekler     | - :ref:`type<reference-constraint-type-type>`                        |
|                | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\Type`           |
+----------------+---------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\TypeValidator`  |
+----------------+---------------------------------------------------------------------+

Temel Kullanım
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                age:
                    - Type:
                        type: integer
                        message: The value {{ value }} is not a valid {{ type }}.

    .. code-block:: php-annotations

       // src/Acme/BlogBundle/Entity/Author.php
       namespace Acme\BlogBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class Author
       {
           /**
            * @Assert\Type(type="integer", message="The value {{ value }} is not a valid {{ type }}.")
            */
            protected $age;
       }

Seçenekler
----------

.. _reference-constraint-type-type:

type
~~~~

**tip**: ``string`` [:ref:`varsayılan seçenek<validation-default-option>`]

Bu gerekli seçenek tam nitelikli sınıf adı ya da PHP'nin belirli ``ìs_`` fonksiyonlarından biri olan
PHP veritiplerinden olabilir.

  * `array <http://php.net/is_array>`_
  * `bool <http://php.net/is_bool>`_
  * `callable <http://php.net/is_callable>`_
  * `float <http://php.net/is_float>`_ 
  * `double <http://php.net/is_double>`_
  * `int <http://php.net/is_int>`_ 
  * `integer <http://php.net/is_integer>`_
  * `long <http://php.net/is_long>`_
  * `null <http://php.net/is_null>`_
  * `numeric <http://php.net/is_numeric>`_
  * `object <http://php.net/is_object>`_
  * `real <http://php.net/is_real>`_
  * `resource <http://php.net/is_resource>`_
  * `scalar <http://php.net/is_scalar>`_
  * `string <http://php.net/is_string>`_
  
message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value should be of type {{ type }}``

Vurgulanan veri verilen tipten değilse gösterilecek mesaj.