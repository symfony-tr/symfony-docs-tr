.. index::
    single: Console; CLI

Console Bileşeni
=====================

    Console bileşeni güzel ve test edilebilir komut satırı arayüzü oluşturmayı kolaylaştırır.

Console bileşeni komut satırı komutları oluşturmanıza izin verir. Konsol komutlarınız yinelenen 
görevler için kullanılabilir, örnek olarak cronjobs, imports veya diğer batch işlemleri.

Kurulum
------------

Bileşen bir çok farklı yolla yüklenebilir:

* Resmi Git reposunu kullanarak (https://github.com/symfony/Console);
* PEAR ile yükleme ( `pear.symfony.com/Console`);
* Composer ile yükleme Composer (Packagist 'deki `symfony/console`).

Kullanım
------------------------

Bize komut satırından selam veren konsol komutu yazmak için, ``GreetCommand.php``
dosyası oluşturun ve aşağıdakileri ekleyin ::

    namespace Acme\DemoBundle\Command;

    use Symfony\Component\Console\Command\Command;
    use Symfony\Component\Console\Input\InputArgument;
    use Symfony\Component\Console\Input\InputInterface;
    use Symfony\Component\Console\Input\InputOption;
    use Symfony\Component\Console\Output\OutputInterface;

    class GreetCommand extends Command
    {
        protected function configure()
        {
            $this
                ->setName('demo:greet')
                ->setDescription('Greet someone')
                ->addArgument('name', InputArgument::OPTIONAL, 'Kimi selamlamak istersin?')
                ->addOption('yell', null, InputOption::VALUE_NONE, 'If set, the task will yell in uppercase letters')
            ;
        }

        protected function execute(InputInterface $input, OutputInterface $output)
        {
            $name = $input->getArgument('name');
            if ($name) {
                $text = 'Merhaba '.$name;
            } else {
                $text = 'Merhaba';
            }

            if ($input->getOption('yell')) {
                $text = strtoupper($text);
            }

            $output->writeln($text);
        }
    }

Ayrıca komut satırında komut çalıştırabilen bir ``Application`` oluşturmak için dosyayı 
oluşturmaya ihtiyacınız var. Komutlar bu dosyaya eklenir::


    #!/usr/bin/env php
    # app/console
    <?php 

    use Acme\DemoBundle\Command\GreetCommand;
    use Symfony\Component\Console\Application;

    $application = new Application();
    $application->add(new GreetCommand);
    $application->run();

Aşağıdaki komutu çalıştırarak yeni konsol komutunuzu test edebilirsiniz

.. code-block:: bash

    app/console demo:greet Fabien

Takip eden yazı komut satırında yazacaktır:

.. code-block:: text

    Merhaba Fabien

``--yell`` opsiyonu ile herşeyi büyük harflerle yazdırabilirsiniz:

.. code-block:: bash

    app/console demo:greet Fabien --yell

Çıktı::

    MERHABA FABIEN

Çıktıyı Renklendirme
~~~~~~~~~~~~~~~~~~~

İstediğiniz zaman çıktı yazısını renklendirmek için etiketleri 
kullanabilirsin. Örneğin::


    // yeşil çıktı
    $output->writeln('<info>foo</info>');

    // sarı çıktı
    $output->writeln('<comment>foo</comment>');

    // mavi arkaplanda siyah yazı
    $output->writeln('<question>foo</question>');

    // kırmızı arkaplanda beyaz yazı
    $output->writeln('<error>foo</error>');

Class 'ları kullanarak kendi stilinizi tanımlamanız mümkün
:class:`Symfony\\Component\\Console\\Formatter\\OutputFormatterStyle`::

    $style = new OutputFormatterStyle('red', 'yellow', array('bold', 'blink'));
    $output->getFormatter()->setStyle('fire', $style);
    $output->writeln('<fire>foo</fire>');

Mevcut ön ve arkaplan renkleri: ``black``, ``red``, ``green``,
``yellow``, ``blue``, ``magenta``, ``cyan`` ve ``white``.

Ve mevcut seçenekler: ``bold``, ``underscore``, ``blink``, ``reverse`` ve ``conceal``.

Komut Argümanları Kullanımı
-----------------------

Komutların en ilgi çekici parçası kullanabileceğiniz argüman ve seçeneklerdir.
Argümanlar komutun kendisinden sonra gelen yazılardır - boşluklarla ayrılır -.
Sıralıdırlar ve isteğe bağlı veya gerekli olabilir. Örnek olarak, komutumuza isteğe bağlı 
``last_name`` argümanı ve gerekli ``name`` argümanı ekleyelim::

    $this
        // ...
        ->addArgument('name', InputArgument::REQUIRED, 'Who do you want to greet?')
        ->addArgument('last_name', InputArgument::OPTIONAL, 'Your last name?')
        // ...

Şimdi ``last_name`` argümanına komutunuzda erişebileceksiniz::

    if ($lastName = $input->getArgument('last_name')) {
        $text .= ' '.$lastName;
    }

Şimdi komut takip eden iki yöntemlede kullanılabilir:

.. code-block:: bash

    app/console demo:greet Fabien
    app/console demo:greet Fabien Potencier

Komut Seçeneklerini Kullanma
---------------------

Argümanların aksine, seçenekler sıralı değildir (yani istediğiniz sıraya 
göre tanımlayabilirsiniz) ve çift çizgi ile tanımlanabilir (Örn. ``--yell`` - 
ayrıca tek çizgi ile de kısayolunu oluşturabilirsiniz Örn. ``-y``).
Seçenekler *herzaman* isteğe bağlıdır, ve belirli bir değer almak için 
ayarlanabilir(Örn. ``dir=src``) veya basitçe boolen işareti ile değer kullanmadan
(Örn. ``yell``).

.. tip::
    
    Bir seçeneği *isteğe bağlı* bir değer alması mümkündür (buna göre 
    ``--yell`` veya ``yell=loud`` ikiside çalışır). Seçenekler bir 
    diziyi kabul edecek şekilde de düzenlenebilir.

Örnek olarak, komutumuza mesajı kaç defa yazdırılacağını tanımlayan 
yeni bir seçenek ekleyelim::

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED, 'Mesaj kaç defa yazılacak?', 1)

Sonra, mesajı birden fazla yazdırmak için bunu kullanın:

.. code-block:: php

    for ($i = 0; $i < $input->getOption('iterations'); $i++) {
        $output->writeln($text);
    }

Şimdi, görevi çalıştırdığınız zaman, isteğe bağlı ``--iterations`` seçeneğini tanımlayabilirsiniz
flag:

.. code-block:: bash

    app/console demo:greet Fabien

    app/console demo:greet Fabien --iterations=5

İlk örnek sadece bir defa yazdıracaktır, ``iterations`` tanımlanmadığı ve 
varsayılan olarak ``1`` ayarlandığı için (``addOption`` ın son argümanı).
İkinci örnek beş defa yazacaktır.

Seçeneklerin sıralamasını önemsemeden geri çağırın. Sonuç olarak, iki türlü 
de komutlar çalışacaktır:

.. code-block:: bash

    app/console demo:greet Fabien --iterations=5 --yell
    app/console demo:greet Fabien --yell --iterations=5

Kullanabileceğiniz 4 seçenek varyasyonu mevcut:

===========================  =====================================================================================
Seçenek                      Değer
===========================  =====================================================================================
InputOption::VALUE_IS_ARRAY  Bu seçenek çoklu değeri kabul eder. (Örn. ``--dir=/foo --dir=/bar``)
InputOption::VALUE_NONE      Girdiyi bu seçenek için kabul etme (Örn. ``--yell``)
InputOption::VALUE_REQUIRED  Bu değer gerekli (Örn. ``--iterations=5``), seçeneğin kendisi halen isteği bağlı
InputOption::VALUE_OPTIONAL  BUu seçeneğin bir değeri olabilir veya olmayabilir (Örn. ``yell`` or ``yell=loud``)
===========================  =====================================================================================

Ayrıca VALUE_IS_ARRAY ile VALUE_REQUIRED veya VALUE_OPTIONAL gibi kombine edebilirsiniz:

.. code-block:: php

    $this
        // ...
        ->addOption('iterations', null, InputOption::VALUE_REQUIRED | InputOption::VALUE_IS_ARRAY, 'How many times should the message be printed?', 1)


Kullanıcıdan Bilgi İsteme
-------------------------------
Komut oluştururken, kullanıcıya sorular sorarak daha fazla bilgi toplanabilir.
Örneğin, komutu çalıştırırken gerçekten emin olduğunu onaylamasını önerebilirsiniz.
Takip eden satırları komutunuza ekleyin::

    $dialog = $this->getHelperSet()->get('dialog');
    if (!$dialog->askConfirmation($output, '<question>Bu işleme devam edilsin mi?</question>', false)) {
        return;
    }

Bu durumda, kullanıcıya "Bu işleme devam edilsin mi?" sorusu sorulacak, ve 
``y`` olarak cevap verimezse, işlem sonlandırılacak. Üçüncü argüman 
``askConfirmation`` için varsayılan cevap yani eğer kullanıcı hiçbir girdi 
yapmazsa ``false`` dönecek.

Ayrıca basit yes/no sorusundan daha kapsamlı bir soru da sorabilirsiniz.Örneğin 
eğer ki birşeyin ismini öğrenmek istiyorsanız, takip edeni yapabilirsiniz::

    $dialog = $this->getHelperSet()->get('dialog');
    $name = $dialog->ask($output, 'Please enter the name of the widget', 'foo');

Test Komutları
----------------

Symfony2 komutunuzu test etmeniz için birkaç araç sağlar. En yararlı olanı 
:class:`Symfony\\Component\\Console\\Tester\\CommandTester` class 'ı. Bu 
gerçek konsol yerine özel girdi ve çıktı class ları kullanarak test eder:: 

    use Symfony\Component\Console\Application;
    use Symfony\Component\Console\Tester\CommandTester;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {
        public function testExecute()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(array('command' => $command->getName()));

            $this->assertRegExp('/.../', $commandTester->getDisplay());

            // ...
        }
    }

Konsoldan normal bir çağrılma süresince ne gösterileceğini 
:method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay` class ı döner.

Bir dizi halinde argüman ve seçenek göndererek komutunuzu test edebilirsiniz. 
:method:`Symfony\\Component\\Console\\Tester\\CommandTester::getDisplay`

method::

    use Symfony\Component\Console\Tester\CommandTester;
    use Symfony\Component\Console\Application;
    use Acme\DemoBundle\Command\GreetCommand;

    class ListCommandTest extends \PHPUnit_Framework_TestCase
    {

        //--

        public function testNameIsOutput()
        {
            $application = new Application();
            $application->add(new GreetCommand());

            $command = $application->find('demo:greet');
            $commandTester = new CommandTester($command);
            $commandTester->execute(
                array('command' => $command->getName(), 'name' => 'Fabien')
            );

            $this->assertRegExp('/Fabien/', $commandTester->getDisplay());
        }
    }

.. tip::
    
    Ayrıca tüm konsol uygulamanızı :class:`Symfony\\Component\\Console\\Tester\\ApplicationTester` 
    sınıfını kullanarak test edebilirsiniz.

Varolan Komutu Çağırmak
---------------------------

Eğer ki bir komut öncesinde çalışan bir komuta dayanıyorsa, kullanıcıya çalıştırma
sırasını hatırlatmak yerine, direk olarak kendi kendine çağırabilirsin. Eğer ki bir 
grup komut ile oluşan "meta" komutları (Örnek olarak proje kodları değiştiği zaman 
çalıştırılması gereken komutlar: cache i temizleme, Doctrine2 proxies üretimi,dumping  
Assetic assets, ...) için kullanımı oldukça yararlıdır.

Komutu başka basit bir komut ile çağırma::

    protected function execute(InputInterface $input, OutputInterface $output)
    {
        $command = $this->getApplication()->find('demo:greet');

        $arguments = array(
            'command' => 'demo:greet',
            'name'    => 'Fabien',
            '--yell'  => true,
        );

        $input = new ArrayInput($arguments);
        $returnCode = $command->run($input, $output);

        // ...
    }

İlk olarak, :method:`Symfony\\Component\\Console\\Application::find` 
metodu ile komutu bir komut ismi ile çağıracağız.

Sonrasında, yenisini oluşturmaya ihtiyacınız var 
:class:`Symfony\\Component\\Console\\Input\\ArrayInput` class ı ile 
argümanlar ve seçenekler gönderebilirsiniz. 

Son olarak, ``run()`` metodu komutu çalıştırır ve komutun çalıştırılmasının 
çıktısını döner (``execute()`` methodunun çıktısını döner).

.. note::
    Çoğu zaman, komutu komut satırı yerine kod üzerinden çalıştırmak iyi bir 
    fikir olmayabilir.İlk olarak, komutların çıktıları komut satırı için 
    optimize edilmiştir. Ama daha önemlisi, komutu bir controller olarak 
    düşünebilirsiniz; kullanıcıya bilgi göstermek için veya birşeyler yapmak 
    için bir model kullanılmalıdır. Bu yüzden ,komutu webden çağırmak yerine,
    kodunuzu yeniden düzenleyin ve yeni bir sınıfa taşıyın.
