input
~~~~~

**tip**: ``string`` **varsayılan**: ``datetime``

*Giriş* (input) verisinin formatıdır. Örneğin ilgili nesnenizde formatlı bir tarih
verisi tutuluyor.Geçerli değerkler:

* ``string`` (örn. ``2012-06-05``)
* ``datetime`` (a ``DateTime`` object)
* ``array`` (örn. ``array('year' => 2012, 'month' => 06, 'day' => 05)``)
* ``timestamp`` (örn. ``1307232000``)

Formdan gelen bilgi bu belirlenen formata göre normalleştirilecektir.
