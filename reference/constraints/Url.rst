Url
===

Bir değerin geçerli bir URL dizesi olduğunu doğrular.

+----------------+---------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`      |
+----------------+---------------------------------------------------------------------+
| Seçenekler     | - `message`_                                                        |
|                | - `protocols`_                                                      |
+----------------+---------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\Url`            |
+----------------+---------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\UrlValidator`   |
+----------------+---------------------------------------------------------------------+

Temel Kullanım
--------------

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author:
            properties:
                bioUrl:
                    - Url:

    .. code-block:: php-annotations

       // src/Acme/BlogBundle/Entity/Author.php
       namespace Acme\BlogBundle\Entity;
       
       use Symfony\Component\Validator\Constraints as Assert;

       class Author
       {
           /**
            * @Assert\Url()
            */
            protected $bioUrl;
       }

Seçenekler
----------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value is not a valid URL``

URL geçersizse bu mesaj gösterilecektir.

protocols
~~~~~~~~~

**tip**: ``array`` **varsayılan**: ``array('http', 'https')``

Geçerli olarak nitelendirilir protokollerdir. Örneğin, eğer ayrıca
``ftp://`` tipinde URL lerin geçerliliğine ihtiyacınız varsa, ``protocols``
dizisini ``http``, ``https``, ve ayrıca ``ftp`` ile listeleyerek yeniden tanımlarsınız.
