.. index::
   single: CSS Selector

CssSelector Bileşeni
=========================

    CssSelector bileşeni CSS seçicilerini  XPath ifadelerine çevirir.

Kurulum
--------

Bu bileşeni üç farklı şekilde kurabilirsiniz:

* Resmi Git reposunu kullanarak (https://github.com/symfony/CssSelector);
* PEAR ile yükleme (`pear.symfony.com/CssSelector`);
* Composer ile yükleme (Packagist 'deki `symfony/css-selector`).

Kullanımı
----------

Neden CSS seçicileri kullanılır?
~~~~~~~~~~~~~~~~~~~~~~

HTML veya bir XML dökümanını ayrıştırma işlemi yaparken, en etkili yöntem 
XPath 'dır.

XPath ifadeleri inanılmaz şekilde esnektir, neredeyse herzaman ihtiyacınız olan 
elementi bulacak bir XPath ifadesi vardır. Ne yazık ki, çok karmaşık olabiliyor, 
ve öğrenme eğrisi çok diktir. Hatta genel işlemler (örneğin belirli bir class 
ile bir elementi bulmak) uzun ve kullanışsız ifadelere ihtiyaç duyabilir.

Birçok geliştirici -- özellikle web geliştiriciler -- elementleri ararken CSS 
seçicilerini kullanmaları daha rahatdır. Hem de stil dosyaları ile, CSS seçicileri 
Javascript 'te ``querySelectorAll`` fonksiyonu ile de kullanılır popüler kütüphaneler 
JQuery, Prototype ve MooTools 'dur.

CSS seçicileri XPath 'a göre daha az güçlüdür, ama yazma, okuma ve anlama yönüyle 
daha kolaydır. Daha az güçlü olduğundan, neredeyse tüm CSS 
seçicileri XPath denkliklerine çevrilebilir. Bu XPath ifadeleri sonrasında diğer 
fonksiyonlar ve sınıflarca bir dökümandan elementleri bulan XPath ifadeleri ile 
kullanılabilir.

``CssSelector`` bileşeni
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Bileşenin tek amacı CSS seçicilerini XPath eşdeğerlerine çevirmektir::

    use Symfony\Component\CssSelector\CssSelector;

    print CssSelector::toXPath('div.item > h4 > a');

Bu takip eden çıktıyı verir:

.. code-block:: text

    descendant-or-self::div[contains(concat(' ',normalize-space(@class), ' '), ' item ')]/h4/a

Bu ifadeyi şu şekildede kullanabilirsiniz, örneğin, :phpclass:`DOMXPath` 
veya :phpclass:`SimpleXMLElement` gibi dökümandaki elementleri bulmak için.

.. tip::
    
    :method:`Crawler::filter()<Symfony\\Component\\DomCrawler\\Crawler::filter>` methodu 
    CSS seçicilerine bağlı elementleri bulmak için ``CssSelector`` bileşenini kullanır. 
    Daha detaylı bilgi için :doc:`/components/dom_crawler` linkine bakın.

CssSelector Bileşeninin Sınırları
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Tüm CSS seçicileri XPath karşılığına çevrilemez.

Sadece web-browser içeriğinde etkili olan birkaç CSS seçicisi vardır.

* link seçicileri: ``:link``, ``:visited``, ``:target``
* kullanıcının hareketlerine bağlı olan seçiciler: ``:hover``, ``:focus``, ``:active``
* Arayüz seçicileri: ``:enabled``, ``:disabled``, ``:indeterminate``
  (ayrıca, ``:checked`` ve ``:unchecked`` vardır)

Pseudo elementleri (``:before``, ``:after``, ``:first-line``,
``:first-letter``) desteklenmemektedir çünkü elementler yerine bir yazının belirli 
bir bölümünü seçer.

Birkaç pseudo sınıfı daha desteklenmemektedir:

* ``:lang(language)``
* ``root``
* ``*:first-of-type``, ``*:last-of-type``, ``*:nth-of-type``,
  ``*:nth-last-of-type``, ``*:only-of-type``. (Bunlar bir element 
  ismi ile çalışır (Örn. ``li:first-of-type``) ama ``*`` ile çalışmaz.
