invalid_message
~~~~~~~~~~~~~~~

**tip**: ``string`` **varsayılan**: ``This value is not valid``

Bu veri doğrulama mesajı eğer form alanında veri düzgün girilmedi ise bunu
doğrulama esnasında çıkan mesaj olarak kullanılır.

Örneğin bu şu şekilde olabilir. Eğer kullanıcı düzgün olmatan bir metni 
:doc:`zaman</reference/forms/types/time>` alanına girerse ve bu metin
düzgün bir şekilde gerçek zaman verisine çevrilmezse (örn:elma) bu bir
hatadır.

Normal(business logic) doğrulama (bir alan için girilmesi gereken en az 
karakter sayısı gibi) mesajı tanımladığınız veri doğrulama kuralları içerisinde
olmalıdır (:ref:`Daha detaylı bilgi için<book-validation-constraint-configuration>`).
