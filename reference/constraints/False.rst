False
=====

Bir değerin ``false`` olduğunu doğrular. Özellikle, değerin kesin olarak ``false``,
kesin olarak tamsayı ``0``, ya da kesin olarak dize "``0``" olup olmadığını kontrol eder.

Ayrıca :doc:`True <True>` 'ya bakınız.

+----------------+---------------------------------------------------------------------+
| Uygulama yeri  | :ref:`sınıf değişkeni ya da metot<validation-property-target>`      |
+----------------+---------------------------------------------------------------------+
| Seçenekler     | - `message`_                                                        |
+----------------+---------------------------------------------------------------------+
| Sınıf          | :class:`Symfony\\Component\\Validator\\Constraints\\False`          |
+----------------+---------------------------------------------------------------------+
| Doğrulayıcı    | :class:`Symfony\\Component\\Validator\\Constraints\\FalseValidator` |
+----------------+---------------------------------------------------------------------+

Temel Kullanım
--------------

``False`` kısıtı bir sınıf değişkenine veya bir "getter" metoduna uygulanabilir,
ancak çoğunlukla sonraki durumlarda kullanışlıdır. Örneğin, farz edin ki bir ``state`` 
sınıf değişkeninin dinamik olan ``invalidStates`` dizisinde *olmadığını* garanti 
etmek istiyorunuz. Önce, bir "getter" metodu yaratırsınız:

    protected $state;

    protected $invalidStates = array();

    public function isStateInvalid()
    {
        return in_array($this->state, $this->invalidStates);
    }

Bu durumda, vurgulanan nesne sadece eğer ``isStateInvalid`` metodu 
**false** dönerse doğrulanacaktır:

.. configuration-block::

    .. code-block:: yaml

        # src/BlogBundle/Resources/config/validation.yml
        Acme\BlogBundle\Entity\Author
            getters:
                stateInvalid:
                    - "False":
                        message: You've entered an invalid state.

    .. code-block:: php-annotations

        // src/Acme/BlogBundle/Entity/Author.php
        use Symfony\Component\Validator\Constraints as Assert;

        class Author
        {
            /**
             * @Assert\False()
             */
             public function isStateInvalid($message = "You've entered an invalid state.")
             {
                // ...
             }
        }

.. caution::

    YAML kullanıldığı zaman, ``False`` 'in etrafının tırnak işaretiyle (``"False"``) sarıldığından emin olun
    aksi takdirde YAML bunu bir Boolean değere çevirecektir.

Seçenekler
----------

message
~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value should be false``

Vurgulanan veri false değilse bu mesaj gösterilecektir.
