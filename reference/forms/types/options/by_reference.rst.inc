by_reference
~~~~~~~~~~~~

**tip**: ``Boolean`` **varsayılan**: ``true``

Pek çok durumda eğer ``name`` alanınız varsa ve ile ilgili nesnede
``setName`` 'in çağırılabilir olmasına gerek vardır. Ancak bazı durumlarda
``setName`` çağırılabilir durumda olmayabilir. ``by_reference`` ile bu 
setter fonksiyonu her durumda çağırılabilir duruma gelir.

Bunu daha iyi anlamak için şu basit örneğe bakalım::

    $builder = $this->createFormBuilder($article);
    $builder
        ->add('title', 'text')
        ->add(
            $builder->create('author', 'form', array('by_reference' => ?))
                ->add('name', 'text')
                ->add('email', 'email')
        )

If ``by_reference`` is true, the following takes place behind the scenes
when you call ``bindRequest`` on the form::

Eğer ``by_reference`` true değerine sahipse formda
``bindRequest`` fonksiyonunu çağırıldığın zaman aşağıdaki şekilde verilere
erişebilirsiniz::

    $article->setTitle('...');
    $article->getAuthor()->setName('...');
    $article->getAuthor()->setEmail('...');

``setAuthor`` 'un çağırılabilir olmadığına dikkat edin. Author referans tarafından
değiştirilir.

Eğer biz ``by_reference`` değerini false yaparsak bağlama şu şekilde olacaktır:

    $article->setTitle('...');
    $author = $article->getAuthor();
    $author->setName('...');
    $author->setEmail('...');
    $article->setAuthor($author);

Yani ``by_reference=false`` gerçekten framework'te üst nesnenin setter'ini
çağırması için zorlayacaktır.

Aynı şekilde eğer ilgili kolleksiyon verisini bir nesne olarak 
:doc:`collection</reference/forms/types/collection>` form tipinde ise (Doctrine'nin
``ArrayCollection`` 'nu gibi) bu durumda eğer setter'ın çağırılabilir 
olmasına ihtiyacınız var ise (örn : ``setAuthors``) ``by_reference`` mutlaka ``false`` 
yapılmalıdır.
