import { Component, inject, input, output } from '@angular/core';
import { Router } from '@angular/router';
import { AuthService } from '../../services/auth.service';

@Component({
  selector: 'app-header',
  imports: [],
  templateUrl: './header.html',
  styleUrl: './header.css'
})
export class Header {

  authService = inject(AuthService);
  menuToggle = output<void>();  // avisa o pai

  constructor(
    private router: Router,) {}
  
  menuAberto = input(false);
  toggleMenu() {
    console.log(this.menuAberto());
    this.menuToggle.emit();
  }

  goHome() {
    this.router.navigate(['/home']);
  }
}
