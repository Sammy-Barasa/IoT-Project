# Generated by Django 3.2.5 on 2021-07-26 07:19

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('DeviceManager', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Room',
            fields=[
                ('id', models.BigAutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('number', models.IntegerField()),
                ('occupied', models.BooleanField()),
                ('lastActivity', models.DateField(auto_now=True)),
            ],
        ),
        migrations.AddField(
            model_name='device',
            name='deviceIDRef',
            field=models.CharField(default='01XYZ', max_length=20),
        ),
        migrations.AlterField(
            model_name='device',
            name='roomNumber',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='DeviceManager.room'),
        ),
    ]