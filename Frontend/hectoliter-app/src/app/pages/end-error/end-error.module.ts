import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';

import { IonicModule } from '@ionic/angular';

import { EndErrorPageRoutingModule } from './end-error-routing.module';

import { EndErrorPage } from './end-error.page';

@NgModule({
  imports: [
    CommonModule,
    FormsModule,
    IonicModule,
    EndErrorPageRoutingModule
  ],
  declarations: [EndErrorPage]
})
export class EndErrorPageModule {}
