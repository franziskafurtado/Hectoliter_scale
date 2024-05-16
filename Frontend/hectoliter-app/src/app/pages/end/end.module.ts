import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { EndPageRoutingModule } from './end-routing.module';

import { EndPage } from './end.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    EndPageRoutingModule
  ],
  declarations: [EndPage]
})
export class EndPageModule {}
