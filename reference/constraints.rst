Veri Doğrulama Koşutları Referansı
==================================

.. toctree::
   :maxdepth: 1
   :hidden:

   constraints/NotBlank
   constraints/Blank
   constraints/NotNull
   constraints/Null
   constraints/True
   constraints/False
   constraints/Type

   constraints/Email
   constraints/MinLength
   constraints/MaxLength
   constraints/Url
   constraints/Regex
   constraints/Ip

   constraints/Max
   constraints/Min

   constraints/Date
   constraints/DateTime
   constraints/Time

   constraints/Choice
   constraints/Collection
   constraints/UniqueEntity
   constraints/Language
   constraints/Locale
   constraints/Country

   constraints/File
   constraints/Image

   constraints/Callback
   constraints/Valid
   constraints/All

Validator nesneleri *koşutlara* göre doğrulayacak şekilde tasarlanmıştır.
Gerçek hayatta bir koşut "Kek yanmamalıdır" şeklide olabilir. Symfony2'de
de koşutlar aynen böyledir. Verilen doğrulama koşutları "doğru" olmalıdır.

Desteklenen Kısıtlar
--------------------

Aşağıdaki kısıtlar Symfony2 'de doğal olarak desteklenir:

.. include:: /reference/constraints/map.rst.inc
