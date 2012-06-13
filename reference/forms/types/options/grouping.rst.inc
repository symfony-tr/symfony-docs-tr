grouping
~~~~~~~~

**tip**: ``integer`` **varsayılan**: ``false``

Bu değer içsel olarak PHP'nin ``NumberFormatter`` sınıfı kullanıldığında
``NumberFormatter::GROUPING_USED``  değerini kullanır. Bunun dokümantasyonu
bulunmamaktadır ancak anlaşıldığı kadarıyla eğer bunu ``true`` değeri yaparsanız
sayılar nokta ya da virgül (,) ile guruplanacaktır (yerel bilginize bağlı
olarak) : ``12345.123`` değeri ``12,345.123`` şeklinde gözükecektir.
